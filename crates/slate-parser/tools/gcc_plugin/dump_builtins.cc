#include <string>
#include <set>
#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tree.h"
#include "tm.h"
#include "target.h"
#include "stringpool.h"
#include "c-family/c-common.h"

int plugin_is_GPL_compatible;

static bool sizeless;
static std::set<std::string> sizeless_names;

static std::string spelling (tree type);

static std::string named (tree type)
{
  tree name = TYPE_NAME (type);
  if (name && TREE_CODE (name) == TYPE_DECL)
    name = DECL_NAME (name);
  return name && TREE_CODE (name) == IDENTIFIER_NODE ? IDENTIFIER_POINTER (name) : "";
}

static std::string qualified (tree type, const std::string &base)
{
  std::string out;
  if (TYPE_READONLY (type))
    out += "const ";
  if (TYPE_VOLATILE (type))
    out += "volatile ";
  return out + base;
}

static std::string spelling (tree type)
{
  tree main = TYPE_MAIN_VARIANT (type);
  std::string name = named (main);
  bool is_unsigned = TYPE_UNSIGNED (main);
  if (TREE_CODE (main) == VECTOR_TYPE ? !TYPE_VECTOR_SUBPARTS (main).is_constant ()
      : (TREE_CODE (main) == RECORD_TYPE && TYPE_SIZE (main)
	 && TREE_CODE (TYPE_SIZE (main)) != INTEGER_CST))
    {
      sizeless = true;
      if (name.empty ())
	name = named (TREE_TYPE (main));
      sizeless_names.insert (name);
      return qualified (type, name);
    }
  switch (TREE_CODE (main))
    {
    case VOID_TYPE:
      return qualified (type, "void");
    case BOOLEAN_TYPE:
      return qualified (type, "bool");
    case INTEGER_TYPE:
    case ENUMERAL_TYPE:
      {
	unsigned bits = TYPE_PRECISION (main);
	std::string base;
	if (bits == 8)
	  base = name == "char" ? "char" : is_unsigned ? "unsigned char" : "signed char";
	else if (bits == 16)
	  base = is_unsigned ? "unsigned short" : "short";
	else if (bits == 32)
	  base = is_unsigned ? "unsigned int" : "int";
	else if (bits == 64)
	  base = name.find ("long long") != std::string::npos
		   ? (is_unsigned ? "unsigned long long int" : "long long int")
		   : (is_unsigned ? "unsigned long int" : "long int");
	else
	  base = is_unsigned ? "__uint128_t" : "__int128_t";
	return qualified (type, base);
      }
    case REAL_TYPE:
      {
	unsigned bits = TYPE_PRECISION (main);
	if (name == "__fp16" || name == "__bf16" || name == "_Float16")
	  return qualified (type, name);
	return qualified (type, bits == 32 ? "float" : bits == 64 ? "double" : "long double");
      }
    case VECTOR_TYPE:
      return qualified (type, "_Vector<" + std::to_string (TYPE_VECTOR_SUBPARTS (main).to_constant ())
				+ ", " + spelling (TREE_TYPE (main)) + ">");
    case POINTER_TYPE:
      return spelling (TREE_TYPE (main)) + "*";
    case RECORD_TYPE:
    case UNION_TYPE:
      if (name.empty () || !TYPE_SIZE (main))
	{
	  sizeless = true;
	  if (!name.empty ())
	    sizeless_names.insert (name);
	}
      return qualified (type, name);
    default:
      sizeless = true;
      return "?";
    }
}

static void dump_functions ()
{
  for (unsigned code = 0; code < (1u << 20); code++)
    {
      tree decl = targetm.builtin_decl (code, true);
      if (!decl || decl == error_mark_node || TREE_CODE (decl) != FUNCTION_DECL
	  || DECL_BUILT_IN_CLASS (decl) != BUILT_IN_MD)
	continue;
      tree fn = TREE_TYPE (decl);
      sizeless = false;
      std::string out = spelling (TREE_TYPE (fn)) + "(";
      bool first = true;
      bool variadic = true;
      for (tree arg = TYPE_ARG_TYPES (fn); arg; arg = TREE_CHAIN (arg))
	{
	  if (TREE_VALUE (arg) == void_type_node)
	    {
	      variadic = false;
	      break;
	    }
	  out += (first ? "" : ", ") + spelling (TREE_VALUE (arg));
	  first = false;
	}
      if (variadic)
	out += first ? "..." : ", ...";
      out += ")";
      const char *attrs = TREE_READONLY (decl) ? "const" : DECL_PURE_P (decl) ? "pure" : "";
      if (!DECL_ATTRIBUTES (decl) && !strncmp (IDENTIFIER_POINTER (DECL_NAME (decl)), "sv", 2))
	attrs = "overload";
      fprintf (stdout, "fn\t%s\t%s\t%s\t%d\n", IDENTIFIER_POINTER (DECL_NAME (decl)), out.c_str (),
	       attrs, sizeless ? 1 : 0);
    }
}

static std::string print_hex_string (tree value)
{
  return std::to_string (tree_to_shwi (value));
}

static const char *type_names;

static void dump_types ()
{
  if (!type_names)
    return;
  FILE *in = fopen (type_names, "r");
  if (!in)
    return;
  char buffer[256];
  while (fscanf (in, "%255s", buffer) == 1)
    {
      std::string name = buffer;
      tree decl = identifier_global_value (get_identifier (name.c_str ()));
      if (!decl)
	continue;
      if (TREE_CODE (decl) == CONST_DECL)
	{
	  fprintf (stdout, "enum\t%s\t%s\t\t0\n", name.c_str (),
		   print_hex_string (DECL_INITIAL (decl)).c_str ());
	  continue;
	}
      if (TREE_CODE (decl) != TYPE_DECL)
	continue;
      tree record = TYPE_MAIN_VARIANT (TREE_TYPE (decl));
      if (TREE_CODE (record) == RECORD_TYPE && TYPE_FIELDS (record)
	  && TREE_CODE (TREE_TYPE (TYPE_FIELDS (record))) == ARRAY_TYPE)
	{
	  tree array = TREE_TYPE (TYPE_FIELDS (record));
	  long count = tree_to_shwi (TYPE_MAX_VALUE (TYPE_DOMAIN (array))) + 1;
	  fprintf (stdout, "tuple\t%s\t%s\t%ld\t0\n", name.c_str (),
		   named (TYPE_MAIN_VARIANT (TREE_TYPE (array))).c_str (), count);
	  continue;
	}
      sizeless = false;
      std::string out = spelling (TREE_TYPE (decl));
      fprintf (stdout, "type\t%s\t%s\t\t%d\n", name.c_str (), out.c_str (), sizeless ? 1 : 0);
    }
  fclose (in);
}

static void marker (void *data, void *)
{
  tree decl = (tree) data;
  if (TREE_CODE (decl) == VAR_DECL && DECL_NAME (decl)
      && !strcmp (IDENTIFIER_POINTER (DECL_NAME (decl)), "slate_dump_marker"))
    dump_types ();
}

static void dump_all (void *, void *)
{
  dump_functions ();
  for (const std::string &name : sizeless_names)
    fprintf (stdout, "type\t%s\t%s\t\t1\n", name.c_str (), name.c_str ());
}

int plugin_init (struct plugin_name_args *info, struct plugin_gcc_version *)
{
  for (int i = 0; i < info->argc; i++)
    if (!strcmp (info->argv[i].key, "types"))
      type_names = info->argv[i].value;
  register_callback ("dump_builtins", PLUGIN_FINISH_DECL, marker, NULL);
  register_callback ("dump_builtins", PLUGIN_FINISH_UNIT, dump_all, NULL);
  return 0;
}

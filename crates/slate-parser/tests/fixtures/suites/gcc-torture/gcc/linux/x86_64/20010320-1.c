// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-additional-options "-fpermissive" } */

typedef struct sec { 
const char *name;
int id;
int index;
struct sec *next;
unsigned int flags;
unsigned int user_set_vma : 1;
unsigned int reloc_done : 1;
unsigned int linker_mark : 1;
unsigned int gc_mark : 1;
unsigned int segment_mark : 1;
unsigned long long vma; } asection;
 
static void pe_print_pdata (asection *section)
{
  unsigned long long i;
  unsigned long long start = 0, stop = 0;
  int onaline = (3*8) ;

  for (i = start; i < stop; i += onaline)
    {
      if (i + (3*8)  > stop)
	break;

      f (((unsigned long) (((   i + section->vma  ) >> 32) & 0xffffffff)) , ((unsigned long) (((   i + section->vma  ) & 0xffffffff))) ) ;
    }
}

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `f`
// DEFAULT: ╭─[tests/fixtures/suites/gcc-torture/gcc/linux/x86_64/20010320-1.c:28:7]
// DEFAULT: 27 │
// DEFAULT: 28 │       f (((unsigned long) (((   i + section->vma  ) >> 32) & 0xffffffff)) , ((unsigned long) (((   i + section->vma  ) & 0xffffffff))) ) ;
// DEFAULT: ·       ─
// DEFAULT: 29 │     }
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT

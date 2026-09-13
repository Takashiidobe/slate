// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-require-effective-target label_values } */

int
main()
{
l1:
  return &&l1-&&l2;
l2:;
}

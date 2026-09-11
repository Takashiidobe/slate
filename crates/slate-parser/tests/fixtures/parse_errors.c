int main( {
  return 3;
}

// SLATE-FILECHECK-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × expected `)`
// DEFAULT: 1 │ int main( {
// DEFAULT: 2 │   return 3;
// SLATE-FILECHECK-END DEFAULT

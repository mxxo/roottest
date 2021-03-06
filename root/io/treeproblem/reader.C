#ifndef ROOT_TFile
#include "TFile.h"
#endif
#ifndef ROOT_TTree
#include "TTree.h"
#endif
#ifndef ROOT_TClonesArray
#include "TClonesArray.h"
#endif
#ifndef ROOT_TROOT
#include "TROOT.h"
#endif
#ifndef __IOSTREAM__
#include <iostream>
#endif
#ifndef ROOT_TSystem
#include "TSystem.h"
#endif

using namespace std;

int reader() 
{
#ifndef ClingWorkAroundMissingDynamicScope
  gSystem->Load("Foo");
#endif

//  gDebug = 2; 

  Int_t  foo_     = 0;
  Int_t  foo_fFoo[3];
  TFile* file     = TFile::Open("file.root", "READ"); 
  TTree* tree     = (TTree*)file->Get("tree");
  tree->SetMakeClass(1);
  tree->SetBranchAddress("foo", (void*)&foo_);
  tree->SetBranchAddress("foo.fFoo", &foo_fFoo);
  
  Int_t  n = (Int_t)tree->GetEntries();

  for (Int_t i = 0; i < n; i++) {
    tree->GetEntry(i);
    for (Int_t j = 0; j < foo_; j++) 
      cout << "Foo # " << i << "," << j << ": " << foo_fFoo[j] << endl;
  }
  file->Close();
  
  return 0;
}

#ifndef __CLING__
int main(int argc, char** argv) 
{
  return reader();
}
#endif


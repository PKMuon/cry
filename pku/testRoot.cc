/*

Copyright (c) 2007-2012, The Regents of the University of California.
Produced at the Lawrence Livermore National Laboratory
UCRL-CODE-227323.
All rights reserved.

For details, see http://nuclear.llnl.gov/simulations
Please also read this http://nuclear.llnl.gov/simulations/additional_bsd.html

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1.  Redistributions of source code must retain the above copyright
notice, this list of conditions and the disclaimer below.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the disclaimer (as noted below) in
the documentation and/or other materials provided with the
distribution.

3. Neither the name of the UC/LLNL nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OF
THE UNIVERSITY OF CALIFORNIA, THE U.S. DEPARTMENT OF ENERGY OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "CRYGenerator.h"
#include "CRYSetup.h"

#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <math.h>
#include <stdlib.h>  // For Ubuntu Linux

int main( int argc, const char *argv[]) {

  if (!TROOT::Initialized()) {
    static TROOT root("RooTuple", "RooTuple ROOT God in CosmicRaY simulation");
  }


  if ( argc < 2 ) {
    std::cout << "usage " << argv[0] << " <setup file name> <N events>\n";
    std::cout << "N events = 10k by default\n";
    return 0;
  }

  TFile *outputFile=new TFile("test.root","RECREATE");

  TH1F* kemup        = new TH1F("kemup",      "kemup",      7*100 + 1,  0.0, 7.0);
  TH1F* kemun        = new TH1F("kemun",      "kemun",      7*100 + 1,  0.0, 7.0);
  TH1F* keneutron    = new TH1F("keneutron",  "keneutron",  7*100 + 1,  0.0, 7.0);
  TH1F* kepip        = new TH1F("kepip",      "kepip",      7*100 + 1,  0.0, 7.0);
  TH1F* kepin        = new TH1F("kepin",      "kepin",      7*100 + 1,  0.0, 7.0);
  TH1F* kepi0        = new TH1F("kepi0",      "kepi0",      7*100 + 1,  0.0, 7.0);
  TH1F* kekaip       = new TH1F("kekaip",     "kekaip",     7*100 + 1,  0.0, 7.0);
  TH1F* kekain       = new TH1F("kekain",     "kekain",     7*100 + 1,  0.0, 7.0);
  TH1F* kekai0       = new TH1F("kekai0",     "kekai0",     7*100 + 1,  0.0, 7.0);
  TH1F* kegamma      = new TH1F("kegamma",    "kegamma",    7*100 + 1,  0.0, 7.0);
  TH1F* keep         = new TH1F("keep",       "keep",       7*100 + 1,  0.0, 7.0);
  TH1F* keen         = new TH1F("keen",       "keen",       7*100 + 1,  0.0, 7.0);
  TH1F* kepp         = new TH1F("kepp",       "kepp",       7*100 + 1,  0.0, 7.0);
  TH1F* kepn         = new TH1F("kepn",       "kepn",       7*100 + 1,  0.0, 7.0);
  TH1F* ctmup        = new TH1F("ctmup",      "ctmup",       1000 + 1, -1.0, 0.0);
  TH1F* ctmun        = new TH1F("ctmun",      "ctmun",       1000 + 1, -1.0, 0.0);
  TH1F* ctneutron    = new TH1F("ctneutron",  "ctneutron",   1000 + 1, -1.0, 0.0);
  TH1F* ctpip        = new TH1F("ctpip",      "ctpip",       1000 + 1, -1.0, 0.0);
  TH1F* ctpin        = new TH1F("ctpin",      "ctpin",       1000 + 1, -1.0, 0.0);
  TH1F* ctpi0        = new TH1F("ctpi0",      "ctpi0",       1000 + 1, -1.0, 0.0);
  TH1F* ctkaip       = new TH1F("ctkaip",     "ctkaip",      1000 + 1, -1.0, 0.0);
  TH1F* ctkain       = new TH1F("ctkain",     "ctkain",      1000 + 1, -1.0, 0.0);
  TH1F* ctkai0       = new TH1F("ctkai0",     "ctkai0",      1000 + 1, -1.0, 0.0);
  TH1F* ctgamma      = new TH1F("ctgamma",    "ctgamma",     1000 + 1, -1.0, 0.0);
  TH1F* ctep         = new TH1F("ctep",       "ctep",        1000 + 1, -1.0, 0.0);
  TH1F* cten         = new TH1F("cten",       "cten",        1000 + 1, -1.0, 0.0);
  TH1F* ctpp         = new TH1F("ctpp",       "ctpp",        1000 + 1, -1.0, 0.0);
  TH1F* ctpn         = new TH1F("ctpn",       "ctpn",        1000 + 1, -1.0, 0.0);

  std::map<std::size_t, TH1F *> keHistos;
  keHistos[((std::size_t)CRYParticle::Muon     << 4) | ((std::size_t) 1 % 4)] = kemup;
  keHistos[((std::size_t)CRYParticle::Muon     << 4) | ((std::size_t)-1 % 4)] = kemun;
  keHistos[((std::size_t)CRYParticle::Neutron  << 4) | ((std::size_t) 0 % 4)] = keneutron;
  keHistos[((std::size_t)CRYParticle::Pion     << 4) | ((std::size_t)+1 % 4)] = kepip;
  keHistos[((std::size_t)CRYParticle::Pion     << 4) | ((std::size_t) 0 % 4)] = kepi0;
  keHistos[((std::size_t)CRYParticle::Pion     << 4) | ((std::size_t)-1 % 4)] = kepin;
  keHistos[((std::size_t)CRYParticle::Kaon     << 4) | ((std::size_t)+1 % 4)] = kekaip;
  keHistos[((std::size_t)CRYParticle::Kaon     << 4) | ((std::size_t) 0 % 4)] = kekai0;
  keHistos[((std::size_t)CRYParticle::Kaon     << 4) | ((std::size_t)-1 % 4)] = kekain;
  keHistos[((std::size_t)CRYParticle::Gamma    << 4) | ((std::size_t) 0 % 4)] = kegamma;
  keHistos[((std::size_t)CRYParticle::Electron << 4) | ((std::size_t)+1 % 4)] = keep;
  keHistos[((std::size_t)CRYParticle::Electron << 4) | ((std::size_t)-1 % 4)] = keen;
  keHistos[((std::size_t)CRYParticle::Proton   << 4) | ((std::size_t)+1 % 4)] = kepp;
  keHistos[((std::size_t)CRYParticle::Proton   << 4) | ((std::size_t)-1 % 4)] = kepn;

  std::map<std::size_t, TH1F *> ctHistos;
  ctHistos[((std::size_t)CRYParticle::Muon     << 4) | ((std::size_t) 1 % 4)] = ctmup;
  ctHistos[((std::size_t)CRYParticle::Muon     << 4) | ((std::size_t)-1 % 4)] = ctmun;
  ctHistos[((std::size_t)CRYParticle::Neutron  << 4) | ((std::size_t) 0 % 4)] = ctneutron;
  ctHistos[((std::size_t)CRYParticle::Pion     << 4) | ((std::size_t)+1 % 4)] = ctpip;
  ctHistos[((std::size_t)CRYParticle::Pion     << 4) | ((std::size_t) 0 % 4)] = ctpi0;
  ctHistos[((std::size_t)CRYParticle::Pion     << 4) | ((std::size_t)-1 % 4)] = ctpin;
  ctHistos[((std::size_t)CRYParticle::Kaon     << 4) | ((std::size_t)+1 % 4)] = ctkaip;
  ctHistos[((std::size_t)CRYParticle::Kaon     << 4) | ((std::size_t) 0 % 4)] = ctkai0;
  ctHistos[((std::size_t)CRYParticle::Kaon     << 4) | ((std::size_t)-1 % 4)] = ctkain;
  ctHistos[((std::size_t)CRYParticle::Gamma    << 4) | ((std::size_t) 0 % 4)] = ctgamma;
  ctHistos[((std::size_t)CRYParticle::Electron << 4) | ((std::size_t)+1 % 4)] = ctep;
  ctHistos[((std::size_t)CRYParticle::Electron << 4) | ((std::size_t)-1 % 4)] = cten;
  ctHistos[((std::size_t)CRYParticle::Proton   << 4) | ((std::size_t)+1 % 4)] = ctpp;
  ctHistos[((std::size_t)CRYParticle::Proton   << 4) | ((std::size_t)-1 % 4)] = ctpn;

  int nEv = 100000;
  if (argc > 2 ) nEv = atoi(argv[2]);

  // Read the setup file into setupString
  std::ifstream inputFile;
  inputFile.open(argv[1],std::ios::in);
  char buffer[1000];

  std::string setupString("");
  while (!inputFile.getline(buffer,1000).eof()) {
    setupString.append(buffer);
    setupString.append(" ");
  }

  // Parse the contents of the setup file
  CRYSetup *setup=new CRYSetup(setupString,"./data");

  // Setup the CRY event generator
  CRYGenerator gen(setup);

  // Generate the events
  int nMuon = 0;
  std::vector<CRYParticle*> *ev=new std::vector<CRYParticle*>;
  for (int i = 0; i < nEv; i++) {
    ev->clear();
    gen.genEvent(ev);

    if (i % 1000 == 0) std::cout << "Event: " << i << std::endl;

    // Fill the histograms
    for (unsigned j = 0; j < ev->size(); j++) {
      CRYParticle *part=(*ev)[j];

      //....printout all secondaries every 1000 events just for fun
      if (i % 1000 == 0) {
        std::cout << "Secondary " << j << " " <<
        CRYUtils::partName(part->id()) << " ke=" << part->ke() << "\n";
      }

      keHistos.at(((std::size_t)part->id() << 4) | ((std::size_t)(int)part->charge() % 4))->Fill(log10(part->ke()));
      ctHistos.at(((std::size_t)part->id() << 4) | ((std::size_t)(int)part->charge() % 4))->Fill(part->w());

      if (part->id() == CRYParticle::Muon) {
        nMuon++;
      }

      delete (*ev)[j];
    }
  }

  std::cout << "Run completed.\n";
  std::cout << "Total time simulated: " << gen.timeSimulated() << " seconds\n";
  double muonsPerSecondPerm2=nMuon/(300.0*300.0*gen.timeSimulated());
  std::cout << "Muons per second per m2 " << muonsPerSecondPerm2 << std::endl;

  // Write the histogram file
  outputFile->Write();
  outputFile->Close();

  delete setup;

  return 0;
}

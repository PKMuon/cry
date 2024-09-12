#include "TFile.h"
#include "TH1.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

using std::operator""s;

void CryPKUGps(const char *histname, const char *parname)
{
  TFile *file = TFile::Open("test.root", "READ");
  if(!file || file->IsZombie()) {
    std::cerr << "无法打开 ROOT 文件!" << std::endl;
    exit(1);
  }

  TH1 *ke = dynamic_cast<TH1 *>(file->Get(("ke"s + histname).c_str()));
  TH1 *ct = dynamic_cast<TH1 *>(file->Get(("ct"s + histname).c_str()));
  if(!ke || !ct) {
    std::cerr << "无法获取直方图!" << std::endl;
    file->Close();
    exit(1);
  }

  std::string opath = "CryGps_"s + parname + ".mac";
  std::ofstream output_file(opath);
  if(!output_file) {
    std::cerr << "无法创建输出文件!" << std::endl;
    file->Close();
    exit(1);
  }
  output_file << std::scientific << std::setprecision(18);

  output_file << "# ----------------------------------------------\n";
  output_file << "# General Particle Source (GPS) Settings\n";
  output_file << "# ----------------------------------------------\n";

  output_file << "\n";
  output_file << "/gps/particle " << parname << "\n";
  output_file << "/gps/ang/type user\n";
  output_file << "/gps/hist/type theta\n";
  for(int i = 1; i <= ct->GetNbinsX(); ++i) {
    float bin_center_ct = ct->GetBinCenter(i);
    float bin_content_ct = ct->GetBinContent(i);
    float angle_in_radians = M_PI - std::acos(bin_center_ct);
    output_file << "/gps/hist/point " << angle_in_radians << " " << bin_content_ct << "\n";
  }

  output_file << "\n";
  output_file << "/gps/ene/type Arb\n";
  output_file << "/gps/hist/type arb\n";
  for(int i = 1; i <= ke->GetNbinsX(); ++i) {
    float bin_center_ke = ke->GetBinCenter(i);
    float bin_content_ke = ke->GetBinContent(i);
    output_file << "/gps/hist/point " << pow(10, bin_center_ke) << " " << bin_content_ke << "\n";
  }
  output_file << "/gps/hist/inter Lin\n";

  output_file.close();
  file->Close();
  std::cout << "数据已保存至 " << opath << std::endl;
}

void CryPKUGps()
{
  CryPKUGps("mup",      "mu+");
  CryPKUGps("mun",      "mu-");
  CryPKUGps("neutron",  "neutron");
  CryPKUGps("pip",      "pi+");
  CryPKUGps("pin",      "pi-");
  CryPKUGps("pi0",      "pi0");
  CryPKUGps("kaip",     "kaon+");
  CryPKUGps("kain",     "kaon-");
  CryPKUGps("kai0",     "kaon0");
  CryPKUGps("gamma",    "gamma");
  CryPKUGps("ep",       "e+");
  CryPKUGps("en",       "e-");
  CryPKUGps("pp",       "proton+");
  CryPKUGps("pn",       "proton-");
  exit(0);
}

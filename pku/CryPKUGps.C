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

  TH1 *keProton = dynamic_cast<TH1 *>(file->Get(histname));
  TH1 *costhe = dynamic_cast<TH1 *>(file->Get("costhe"));
  if(!keProton || !costhe) {
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
  for(int i = 1; i <= costhe->GetNbinsX(); ++i) {
    float bin_center_costhe = costhe->GetBinCenter(i);
    float bin_content_costhe = costhe->GetBinContent(i);
    float angle_in_radians = M_PI - std::acos(bin_center_costhe);
    output_file << "/gps/hist/point " << angle_in_radians << " " << bin_content_costhe << "\n";
  }

  output_file << "\n";
  output_file << "/gps/ene/type Arb\n";
  output_file << "/gps/hist/type arb\n";
  for(int i = 1; i <= keProton->GetNbinsX(); ++i) {
    float bin_center_keProton = keProton->GetBinCenter(i);
    float bin_content_keProton = keProton->GetBinContent(i);
    output_file << "/gps/hist/point " << bin_center_keProton << " " << bin_content_keProton << "\n";
  }
  output_file << "/gps/hist/inter Lin\n";

  output_file.close();
  file->Close();
  std::cout << "数据已保存至 " << opath << std::endl;
}

void CryPKUGps()
{
  CryPKUGps("keMuon", "mu-");
  CryPKUGps("keNeutron", "neutron");
  CryPKUGps("kePion", "pion");
  CryPKUGps("keKaon", "kaon");
  CryPKUGps("keGamma", "gamma");
  CryPKUGps("keElectron", "e-");
  CryPKUGps("keProton", "proton");
  exit(0);
}

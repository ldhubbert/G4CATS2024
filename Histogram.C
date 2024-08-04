void Histogram()
{
	TCanvas *c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
	c1->Divide(1,1);
	c1->cd(1);
	
	TString filename = "~/G4CATS2024_rootinput/RUN_RESULTS.root";
	TFile *f = TFile::Open(filename);

	//Cannot start at 0
	TH1F *h1 = new TH1F("Histogram Statistics", "", 1000, 1, 1000);

	TTreeReader r1("B4", f);
	TTreeReaderValue<Double_t> Ecore(r1, "Ecore");
	TTreeReaderValue<Double_t> Eann1(r1, "Eann1");
	TTreeReaderValue<Double_t> Eann2(r1, "Eann2");
	TTreeReaderValue<Double_t> Eann3(r1, "Eann3");
	TTreeReaderValue<Double_t> Eann4(r1, "Eann4");
	TTreeReaderValue<Double_t> Eann5(r1, "Eann5");
	TTreeReaderValue<Double_t> Eann6(r1, "Eann6");
 
        //We multiply by 1000 in the while loop because the output from EvGen is in GeV, and we want the value in MeV.
	//1000*GeV = value in MeV	
	while (r1.Next()) 
	{
		h1->Fill((*Ecore + *Eann1 + *Eann2 + *Eann3 + *Eann4 + *Eann5 + *Eann6)*(1000));
		//h1->Fill(*Ecore + *Eann1 + *Eann2 + *Eann3 + *Eann4 + *Eann5 + *Eann6);
	}
	
	h1->GetXaxis()->SetTitle("Energy (MeV)");
	h1->GetYaxis()->SetTitle("Counts");
	h1->GetXaxis()->CenterTitle();
	h1->GetYaxis()->CenterTitle();
	h1->SetTitle("300MeV Incident Beam");
	h1->Draw();
}

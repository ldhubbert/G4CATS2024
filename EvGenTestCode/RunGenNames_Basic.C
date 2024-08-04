//Understanding what the third argument of the h1 TNtuple is in the EvGenBasic source code
//I wanted to know what this code did with the two parameters, npart = 1 and ptag = {1}, since they would be the two parameters for a particle beam fired straight into CATS.
//It turns out that this outputs the column names of the TNtuple tabular data, where each column represents a different variable (i.e., x-position, y-position, etc.) in a specific string format, to be read in properly by the TNtuple h1 in the EvGen source code.

TString GenNames(UInt_t npart, UInt_t* ptag)
{

	UInt_t i, j;

	TString pstr[] = {"Px", "Py", "Pz", "Pt", "En"};
	TString beam = "X_vtx:Y_vtx:Z_vtx:Px_bm:Py_bm:Pz_bm:Pt_bm:En_bm";
	TString particles;
	TString names;

	for (i = 0; i < npart; i++)
	{

		for (j = 0; j < 5; j++)
		{

			particles.Append(pstr[j]);
			if ((i == (npart-1)) && (j == 4))
			{

				particles.Append(Form("_l%02d%02d", i+1, ptag[i]));

			}
			else
			{

				particles.Append(Form("_l%02d%02d:", i+1, ptag[i]));

			}

		}

	}

	names = beam + ":" + particles;

	return names;

}

void RunGenNames()
{

	UInt_t npart = 1;
	UInt_t ptag[1] = {1};

	//Specifies that the return of the method "GenNames" is a TString named "names"
	TString names = GenNames(npart, ptag);

	cout << "Generated Names: " << names << endl;

}

int main()
{

	RunGenNames();

	return 0;

}

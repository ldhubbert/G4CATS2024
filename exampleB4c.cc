/// \file exampleB4c.cc
/// \brief Main program of the B4c example

#include "B4cDetectorConstruction.hh"
#include "B4cActionInitialization.hh"
#include "B4PrimaryGeneratorAction.hh"
#include "B4File.hh"

#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "FTFP_BERT.hh"
#include "Randomize.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "TFile.h"
#include "TNtuple.h"

TFile *f = new TFile("~/EvGen/out/10cm/compton_c_300_in.root");
TNtuple *n = (TNtuple*)f->Get("h1");
const Int_t number_of_events = n->GetEntries();

bool hardcode = false;
G4String macro;

int main(int argc,char** argv)
{	
  
  //Setting up variables and initializations
  
  G4String session;

  G4RunManager * runManager = new G4RunManager;

  //Set manadatory initialization classes
  auto detConstruction = new B4cDetectorConstruction();
  runManager->SetUserInitialization(detConstruction);

  auto physicsList = new FTFP_BERT;
  runManager->SetUserInitialization(physicsList);

  auto actionInitialization = new B4cActionInitialization();
  runManager->SetUserInitialization(actionInitialization);

  //Initialize visualization
  auto visManager = new G4VisExecutive;
  visManager->Initialize();

  //Get the pointer to the User Interface session
  auto UImanager = G4UImanager::GetUIpointer();

  //End of setting up variables and initializations
  //Start of defining how input into the terminal controls the simulation

  //this goes through the commands you have written in the command line
  //i.e., if you write ./build/exampleB4c -m run1.mac, the simulation is argument 0, "-m" is argument 1, and "run1.mac" is argument 2
  //However, I just want the loop to recognize the command, i.e., "-m", and don't care about what is written after that.
  //So, I will just go through the foor loop once.
  
  for (G4int i = 1; i < 2; i++)
  {
    if ( G4String(argv[i]) == "-m" )
    {
    macro = argv[i+1]; 
    }
    else if ( G4String(argv[i]) == "-u" )
    {
    session = argv[i+1];
    }
    else if ( G4String(argv[i]) == "-h" )
    {
    hardcode = true;
    }

  } 

  //"-u" option is to initiate the GUI!
  //Session "G4UIXm" works as the string in the command line

  //Detect hard-coded material
  if ( hardcode )
  {
  runManager->Initialize();
  runManager->BeamOn(number_of_events);
  }

  //If not hardcode, process macro
  else if ( macro.size() )
  {
  //batch mode
  G4String command = "/control/execute ";
  UImanager->ApplyCommand(command+macro);
  }

  //if not hardcode or macro, maybe a UI session has been defined.
  else if ( session.size() )
  {

  G4UIExecutive* ui = 0;
  ui = new G4UIExecutive(argc, argv, session);
  
  UImanager->ApplyCommand("/control/execute init_vis.mac");
  
  //if (ui->IsGui())
  //{UImanager->ApplyCommand("/control/execute gui.mac");}

  ui->SessionStart();
  delete ui;

  }

  //There is no else statement, so if no hardcode, macro, or interactive modes have been specified, nothing would happen.

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !

  delete visManager;
  delete runManager;
}


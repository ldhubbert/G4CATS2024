/// \file B4PrimaryGeneratorAction.cc
/// \brief Implementation of the B4PrimaryGeneratorAction class

#include "B4PrimaryGeneratorAction.hh"
#include "B4File.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "TLorentzVector.h"
#include "TFile.h"
#include "G4ThreeVector.hh"
#include "TNtuple.h"

B4PrimaryGeneratorAction::B4PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr)
{

  //initializes the Particle Gun
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

}

B4PrimaryGeneratorAction::~B4PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void B4PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume 
  // from G4LogicalVolumeStore
  //
  G4double worldZHalfLength = 0.;//was 0
  auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

  // Check that the world volume has box shape
  G4Box* worldBox = nullptr;
  if (  worldLV ) {
    worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  }

  if ( worldBox ) {
    worldZHalfLength = worldBox->GetZHalfLength();
  }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found." << G4endl;
    msg << "Perhaps you have changed geometry." << G4endl;
    msg << "The gun will be place in the center.";
    G4Exception("B4PrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002", JustWarning, msg);
  } 
  
  if (hardcode)
  {

  //Since this file is re-implemented for each event, we must know what number event we are on to properly direct it to the correct particle information.
  //Since it is a static variable, it is remembered (but the initial declaration counter = 0 is forgotten about after.)
  //This is used as a kind of for loop.

  static int counter = 0;
  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particleDefinition = particleTable->FindParticle("gamma");
  fParticleGun->SetParticleDefinition(particleDefinition);

  Float_t x_position[number_of_events];
  Float_t y_position[number_of_events];
  Float_t z_position[number_of_events];

  Float_t momentum_in_x_direction[number_of_events];
  Float_t momentum_in_y_direction[number_of_events];
  Float_t momentum_in_z_direction[number_of_events];

  //Loads the data for the row corresponding to the number "counter"
  n->GetEntry(counter);

  //Particle Position
  x_position[counter] = n->GetArgs()[0];
  y_position[counter] = n->GetArgs()[1];
  z_position[counter] = n->GetArgs()[2];

  //std::cout << "x_position[counter]: " << x_position[counter] << std::endl;

  //G4ThreeVector position(0, 0, 0);
  G4ThreeVector position(x_position[counter], y_position[counter], z_position[counter]);
  //std::cout << "Particle Position: " << position << std::endl;
  fParticleGun->SetParticlePosition(position);

  //Particle Momentum Direction
  momentum_in_x_direction[counter] = n->GetArgs()[13];
  momentum_in_y_direction[counter] = n->GetArgs()[14];
  momentum_in_z_direction[counter] = n->GetArgs()[15];

  G4ThreeVector momentum_direction(momentum_in_x_direction[counter], momentum_in_y_direction[counter], momentum_in_z_direction[counter]);
  //std::cout << "Particle Momentum Direction: " << momentum_direction << std::endl;
  fParticleGun->SetParticleMomentumDirection(momentum_direction);

  //Particle Energy
  Float_t energy = n->GetArgs()[17];
  //std::cout << "Particle Energy: " << energy <<std::endl;
  fParticleGun->SetParticleEnergy(energy);


  //Produce an event!!
  fParticleGun->GeneratePrimaryVertex(anEvent);

  counter++;
  //std::cout << "static incremented: " << counter << std::endl;
  
  //Information about the TNtuple in normal EvGen:

  //Where, in our TNtuple, each row corresponds to an event, and each column represents a different variable
  //For Compton scattering off of C12, those columns are:
  //
  //0-2: interaction vertex positions
  //3-7: incident photon beam
  //8-12: recoil nucleus
  //13-17: scattered particle
  //
  //Column 0: Interaction x-vertex position --- X_vtx
  //Column 1: Interaction y-vertex position --- Y_vtx
  //Column 2: Interaction z-vertex position --- Z_vtx
  //Column 3: x-momentum of incoming photon beam (direction cosine) --- Px_bm
  //Column 4: y-momentum of incoming photon beam (direction cosine) --- Py_bm
  //Column 5: z-momentum of incoming photon beam (direction cosine) --- Pz_bm
  //NOTE: In the EvGen code, columns 6 and 7 are equal due to photon beam properties.
  //Column 6: total incident photon beam momentum --- Pt_bm
  //COlumn 7: incident photon beam energy (using Bremsstrahlung dist.) --- En_bm
  //Column 8: x-momentum of recoil nucleus --- Px_l0167 (the letter "l" is in front; that's not a one)
  //Column 9: y-momentum of recoil nucleus --- Py_l0167
  //Column 10: z-momentum of recoil nucleus --- Pz_l0167
  //Column 11: total recoil nucleus momentum --- Pt_l0167
  //Column 12: energy of recoil nucleus --- En_l0167
  //Column 13: x-momentum of scattered particle --- Px_l0201
  //Column 14: y-momentum of scattered particle --- Py_l0201
  //Column 15: z-momentum of scattered particle --- Pz_l0201
  //Column 16: total scattered particle momentum --- Pt_l0201
  //Column 17: energy of scattered particle --- En_l0201
  //For a total of 18 columns.
  //
  //Direction cosines need to be multiplied by the total momentum in order to get the momentum in that direction.
  //Direction cosines are numbers between -1 and 1.

  }

  else if (macro.size())
  {

  //The following line needs to be in the code, even if you are running a macro file ("./build/exampleB4c -m run1.mac")
  //If you delete this line, the computer will get mad at you :(  
  //Generate an event
  fParticleGun->GeneratePrimaryVertex(anEvent);

  }

}



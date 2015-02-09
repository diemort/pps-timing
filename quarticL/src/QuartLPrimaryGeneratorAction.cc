#include "QuartLPrimaryGeneratorAction.hh"

using namespace CLHEP;

QuartLPrimaryGeneratorAction::QuartLPrimaryGeneratorAction() :
  fFile(0), fTree(0)
{
  G4int n_particle = 1;
  
  //G4double z_mv = G4UniformRand() * 0.1*cm;  
  //G4double y_mv = G4UniformRand() * 0.1*cm;  
  
  fParticleGun = new G4ParticleGun(n_particle);
  
  //create a messenger for this class
  fGunMessenger = new QuartLPrimaryGeneratorMessenger(this);
  
  //default kinematic
  //
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  // G4ParticleDefinition* particle = particleTable->FindParticle("e+");
  //   G4ParticleDefinition* particle = particleTable->FindParticle("e-");  
  G4ParticleDefinition* particle = particleTable->FindParticle("proton");   
  
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleTime(0.0*ns);
  //  fParticleGun->SetParticlePosition(G4ThreeVector(-10.0*cm,0.0*cm,0.0*cm));
  //    fParticleGun->SetParticlePosition(G4ThreeVector(-5.0*cm,0.0*cm,0.0*cm));  
  //    fParticleGun->SetParticlePosition(G4ThreeVector(-5.0*cm, 0.0*cm, 0.0*cm)); //was Z=0.0 
  //    fParticleGun->SetParticlePosition(G4ThreeVector(-5.0*cm, y_mv,   z_mv)); 	//was Z=0.0 
  //------------------------was ^ now --------------------
  //
  //    was    fParticleGun->SetParticlePosition(G4ThreeVector(0*cm, 0*cm ,    -5.0*cm)); 
  fParticleGun->SetParticlePosition(G4ThreeVector(1*mm, 1*mm, -5.0*cm));
  //
  //	Beam under Angle, 26.04
  //
  //was 2 jun        fParticleGun->SetParticlePosition(G4ThreeVector(-0.1*cm, 0*cm ,    -5.0*cm)); 	
  
  
  //Run39 fParticleGun->SetParticlePosition(G4ThreeVector(0.1*cm,0.1*cm , -5.0*cm)); 
  //Run41 fParticleGun->SetParticlePosition(G4ThreeVector(-0.1*cm,0.1*cm , -5.0*cm)); 
  
  
  //  fParticleGun->SetParticlePosition(G4ThreeVector(-5.0*cm, 0.0*cm,-1.0*cm)); 	//was Z=0.0 
  //  fParticleGun->SetParticlePosition(G4ThreeVector(-5.0*cm, 0.0*cm, 1.0*cm)); 	//was Z=0.0         
  
  //  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  //---------------------was now ^----------------------  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));  
  //  
  //	Beam under Angle, 26.04. 2012  
  //  
  //Run82  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.025,0.,0.9994));  
  //           fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0,0.025,0.9994)); //Run85  
  //was 2 jun  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.025,0.0,0.9994)); //Run86  	 
  
  fParticleGun->SetParticleEnergy(2000.*GeV);
  //  fParticleGun->SetParticleEnergy(2500.0*keV); 
}

QuartLPrimaryGeneratorAction::~QuartLPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;
}

void
QuartLPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  G4double x, y, z;
  if (fHasEventsCollection) {
    G4int event_id = (G4int)(G4UniformRand()*fEventsCollectionSize); // We pickup a random event in the collection
    G4double proton_speed;
    for (G4int hit=0; hit<fNumHits[event_id]; hit++) {
      proton_speed = CLHEP::c_light*sqrt(1.-std::pow(fParticleGun->GetParticleDefinition()->GetPDGMass()/fE[event_id][hit], 2));
      /*x = fY[event_id][hit];
      y = fDetectorBox->GetXHalfLength()+fX[event_id][hit]+2.*mm;
      z = detector_z_boundary-proton_speed*(fArrivalTime[event_id][hit]*ns)+198.*m;*/
      x = fX[event_id][hit]*mm;
      y = fY[event_id][hit]*mm;
      //z = detector_z_boundary;
      z = -5.*cm;
      //G4cout << hit << ": " << fArrivalTime[event_id][hit]*ns*proton_speed << G4endl;
      //G4cout << "speed, z0 for proton " << hit << " = " << proton_speed/CLHEP::c_light << ", " << z/m << G4endl;
      fParticleGun->SetParticleEnergy(fE[event_id][hit]*GeV);
      fParticleGun->SetParticlePosition(G4ThreeVector(x,y,z));
      fParticleGun->GeneratePrimaryVertex(event);
      //G4cout << " -> now " << anEvent->GetNumberOfPrimaryVertex() << " vertices in the event # " << anEvent->GetEventID() << G4endl;
      /*G4PrimaryVertex *vtx = anEvent->GetPrimaryVertex(anEvent->GetNumberOfPrimaryVertex()-1); // get the last vertex generated for this event
      // Update the vertex info (and make sure it exists).
      GastofPrimaryVertex* vInfo = static_cast<GastofPrimaryVertex*>(vtx->GetUserInformation());
      if (!vInfo) {
        vInfo = new GastofPrimaryVertex();
        vtx->SetUserInformation(vInfo);
      }
      vInfo->SetProtonRole(fIsSignal[event_id][hit]);*/
    }
  }
  else {
    fParticleGun->GeneratePrimaryVertex(event);
  }
}

void
QuartLPrimaryGeneratorAction::SetOptPhotonPolar()
{
 G4double angle = G4UniformRand() * 360.0*deg;
 SetOptPhotonPolar(angle);
}

void
QuartLPrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
{
  if (fParticleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton") {
    G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
      "the particleGun is not an opticalphoton" << G4endl;
    return;
  }
  
  G4ThreeVector normal (1., 0., 0.);
  G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
  G4ThreeVector product = normal.cross(kphoton); 
  G4double modul2       = product*product;
  
  G4ThreeVector e_perpend (0., 0., 1.);
  if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product; 
  G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
  
  G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
  fParticleGun->SetParticlePolarization(polar);
}

G4bool
QuartLPrimaryGeneratorAction::SetInputROOTFile(G4String filename)
{
  fFile = new TFile(filename, "READ");
  if (!fFile->IsOpen()) {
    return false;
  }
  
  fTree = (TTree*)(fFile->Get("hits")->Clone());
  if (!fTree) {
    return false;
  }

  const G4int entries = (G4int)fTree->GetEntries();
  const G4int max_hits = 10;
  G4int hits;
  G4double x[max_hits], y[max_hits], energy[max_hits], arrival_time[max_hits];
  G4int signal[max_hits];
  
  fX = new G4double*[entries];
  fY = new G4double*[entries];
  fE = new G4double*[entries];
  fArrivalTime = new G4double*[entries];
  fIsSignal = new G4int*[entries];
  fNumHits = new G4int[entries];
  
  fTree->SetBranchAddress("num_hits", &hits);
  fTree->SetBranchAddress("x", x);
  fTree->SetBranchAddress("y", y);
  fTree->SetBranchAddress("E", energy);
  fTree->SetBranchAddress("arrival_time", arrival_time);
  fTree->SetBranchAddress("signal", signal);
  
  for (G4int i=0; i<entries; i++) {
    fTree->GetEntry(i);
    
    const G4int c_hits = hits;
    fX[i] = new G4double[c_hits];
    fY[i] = new G4double[c_hits];
    fE[i] = new G4double[c_hits];
    fArrivalTime[i] = new G4double[c_hits];
    fIsSignal[i] = new G4int[c_hits];
    fNumHits[i] = c_hits;
    
    for (G4int j=0; j<c_hits; j++) {
      fX[i][j] = x[j];
      fY[i][j] = y[j];
      fE[i][j] = energy[j];
      fArrivalTime[i][j] = arrival_time[j];
      fIsSignal[i][j] = signal[j];
    }
  }
  G4cout << "[QuartLPrimaryGeneratorAction::SetEventsCollection] Loaded a " << entries << " events collection" << G4endl;
  fHasEventsCollection = true;
  fEventsCollectionSize = entries;
  
  fFile->Close();
  delete fFile;
  
  return true;
}

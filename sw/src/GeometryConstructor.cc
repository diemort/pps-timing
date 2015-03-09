#include "GeometryConstructor.hh"

using namespace CLHEP;

namespace PPS
{
  GeometryConstructor::GeometryConstructor()
  {
    //expHall_x = expHall_y = expHall_z = 1*m;
    expHall_x = 30*cm; // was 10
    expHall_y = 30*cm; // was 10
    expHall_z = 50*cm; // was 10
    
    fMessenger = new GeometryConstructorMessenger(this);
    fMaterial = new MaterialManager;
  }
  
  GeometryConstructor::~GeometryConstructor()
  {
    delete fMessenger;
    delete fMaterial;
  }
  
  G4VPhysicalVolume*
  GeometryConstructor::Construct()
  {
    // We build the detector geometry (along with the
    // sensitive detectors associated to it)
    return ConstructGeometry();
  }
  
  G4VPhysicalVolume*
  GeometryConstructor::ConstructGeometry()
  {
    // Clean old geometry, if any
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();
    
    // Experimental hall
    G4Box* expHall_box = new G4Box("World", expHall_x, expHall_y, expHall_z);
    
    expHall_log = new G4LogicalVolume(
      expHall_box,
      fMaterial->Air,
      "World",
      0, 0, 0);

    expHall_phys = new G4PVPlacement(0, G4ThreeVector(), expHall_log, "World", 0, false, 0);
        
    std::vector<std::string> comp = ComponentsStore::GetInstance()->GetRegisteredComponents();
    G4cout << __PRETTY_FUNCTION__  << " : " << ComponentsStore::GetInstance()->GetNumRegisteredComponents() << " components registered :"<< G4endl;
    for (std::vector<std::string>::iterator it=comp.begin(); it!=comp.end(); it++) {
      G4cout << "--> " << *it << G4endl;
    }
    
    // First we unlock the output file to allow the addition of
    // events placeholders from our sensitive detectors
    RunAction* run = (RunAction*)G4RunManager::GetRunManager()->GetUserRunAction();
    run->GetFileWriter()->GetEventInformation()->UnLock();
    
    G4int i=0;
    for (ComponentsRef::iterator det=fComponents.begin(); det!=fComponents.end(); det++, i++) {
      //if (fComponentsBuilt.at(i)) continue;
      G4cout << __PRETTY_FUNCTION__ << " building detector " << i << " at " << fComponentsLocation[i] << G4endl;
      (*det)->SetCenter(fComponentsLocation.at(i));
      (*det)->SetParentLog(expHall_log);
      (*det)->SetParentPhys(expHall_phys);
      (*det)->BeforeConstruct();
      (*det)->Construct();
      //G4cout << ((QuartLDetector*)(*det))->GetCellCenter(2) << G4endl;
      fComponentsBuilt.at(i) = true;
    }
    
    // Finally we lock the output file to avoid the addition of
    // events placeholders from additional sensitive detectors
    run->GetFileWriter()->GetEventInformation()->Lock();

    return expHall_phys;
  }

  G4int
  GeometryConstructor::AddNewComponent(G4String type)
  {
    G4cout << __PRETTY_FUNCTION__ << " --> Let's add a \"" << type << "\", shall we ?" << G4endl;
    if (type=="QUARTIC") {
      std::ostringstream ss; ss << "quartic_" << fComponents.size();
      fComponents.push_back((GeometryComponent*)(new Quartic::QuartLDetector(ss.str())));
    }
    else if (type=="MBP") {
      std::ostringstream ss; ss << "mbp_" << fComponents.size();
      fComponents.push_back((GeometryComponent*)(new MBP::MBP(ss.str())));
    }
    else return -1;
    
    // By default the component is set to the origin
    fComponentsLocation.push_back(G4ThreeVector());
    fComponentsBuilt.push_back(false);
    
    return fComponents.size()-1;
  }
  
  G4bool
  GeometryConstructor::MoveComponent(G4int id, G4ThreeVector pos)
  {
    if ((id<0) or ((size_t)id>=fComponentsLocation.size())) return false;
    fComponentsLocation.at(id) = pos;
    return true;
  }
  
  G4bool
  GeometryConstructor::RotateComponentTheta(G4int id, G4double theta)
  {
    if ((id<0) or ((size_t)id>=fComponentsLocation.size())) return false;
    fComponents.at(id)->RotateTheta(theta);
    return true;
  }
  
  G4bool
  GeometryConstructor::RotateComponentPhi(G4int id, G4double phi)
  {
    if ((id<0) or ((size_t)id>=fComponentsLocation.size())) return false;
    fComponents.at(id)->RotatePhi(phi);
    G4cout << __PRETTY_FUNCTION__ << " -> " << phi << G4endl;
    return true;
  }
  
  G4bool
  GeometryConstructor::SetSDname(G4int id, G4String name)
  {
    if ((id<0) or ((size_t)id>=fComponentsLocation.size())) return false;
    fComponents.at(id)->SetSDname(name);
    return true;
  }
  
  void
  GeometryConstructor::UpdateGeometry()
  {
    G4cout << __PRETTY_FUNCTION__ << " ====> Geometry update !" << G4endl;
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructGeometry());
  }
  
  void
  GeometryConstructor::WriteGDML(G4String filename)
  {
    G4GDMLParser parser;
    parser.Write(filename, expHall_phys, true, "gdml/gdml.xsd");
  }
}

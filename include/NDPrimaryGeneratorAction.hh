#ifndef NDPrimaryGeneratorAction_h
#define NDPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4Event;


class NDPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  NDPrimaryGeneratorAction();    
  ~NDPrimaryGeneratorAction();

public:
  virtual void GeneratePrimaries(G4Event*);
  G4ParticleGun* GetParticleGun() {return fParticleGun;};

private:
  G4ParticleGun*  fParticleGun;        //pointer a to G4 service class
};


#endif

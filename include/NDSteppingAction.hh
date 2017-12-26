
#ifndef NDSteppingAction_h
#define NDSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4Step;
class NDRecorder;

class NDSteppingAction : public G4UserSteppingAction
{
public:
    NDSteppingAction(NDRecorder *);
    ~NDSteppingAction();

    void UserSteppingAction(const G4Step *);

private:
    NDRecorder *recorder;
};

#endif

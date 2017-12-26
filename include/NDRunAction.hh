#ifndef NDRunAction_h
#define NDRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class NDRecorder;


class NDRunAction : public G4UserRunAction
{
public:
    NDRunAction(NDRecorder *);
    ~NDRunAction();

    void BeginOfRunAction(const G4Run *);
    void   EndOfRunAction(const G4Run *);

private:
    NDRecorder *recorder;
};


#endif

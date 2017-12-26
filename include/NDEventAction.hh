
#ifndef NDEventAction_h
#define NDEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;
class NDRecorder;


class NDEventAction : public G4UserEventAction
{
public:
    NDEventAction(NDRecorder *);
    ~NDEventAction();

    void BeginOfEventAction(const G4Event *);
    void EndOfEventAction(const G4Event *);

private:
    NDRecorder *recorder;
};


#endif

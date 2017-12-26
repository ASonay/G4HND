#ifndef NDRecorderMessenger_hh
#define NDRecorderMessenger_hh 1

#include "G4UImessenger.hh"
#include "globals.hh"


class NDRecorder;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

class NDRecorderMessenger:public G4UImessenger
{
public:
    NDRecorderMessenger(NDRecorder *);
    ~NDRecorderMessenger();

    void SetNewValue(G4UIcommand *, G4String);

private:
    NDRecorder             *Recorder;

    G4UIdirectory             *RecMsg;
    G4UIcmdWithAString 		  *FileName;
};

#endif

#ifndef _DARWIN_SESSION_H_
#define _DARWIN_SESSION_H_

class CoreSession
{
private:
public:
    enum HUDSelection
    {
        MutationRate,
        Speed
    };
    static int extensionId;
    static int sessionId;
    static int generationId;
    static int generationIndex;
    static float speed;
    static HUDSelection hudSelection;
};

#endif // _DARWIN_SESSION_H_
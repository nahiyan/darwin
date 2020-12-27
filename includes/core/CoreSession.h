#ifndef _DARWIN_SESSION_H_
#define _DARWIN_SESSION_H_

class CoreSession
{
private:
public:
    static int extensionId;
    static int sessionId;
    static int generationId;
    static int generationIndex;
    static float speed;
};

#endif // _DARWIN_SESSION_H_
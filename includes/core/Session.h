#ifndef _CORE_SESSION_H_
#define _CORE_SESSION_H_

namespace Core
{
    class Session
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
} // namespace Core

#endif // _CORE_SESSION_H_
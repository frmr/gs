#include "gsCulture.h"

string gs::Culture::GenerateCultureName( const int groupId )
{
    switch ( groupId )
    {
        case 0:     return "British";
        case 1:     return "French";
        case 2:     return "Spanish";
        case 3:     return "German";
        case 4:     return "Italian";
        case 5:     return "Swedish";
        case 6:     return "Polish";
        case 7:     return "Balkan";
        case 8:     return "Russian";
        case 9:     return "Turkish";
        case 10:    return "Arab";
        case 11:    return "Persian";
        case 12:    return "Kazakh";
        case 13:    return "Siberian";
        case 14:    return "Siberian";
        case 15:    return "Tibetan";
        case 16:    return "Hindi";
        case 17:    return "Dravidian";
        case 18:    return "Burmese";
        case 19:    return "Thai";
        case 20:    return "Malaysian";
        case 21:    return "Chinese";
        case 22:    return "Chinese";
        case 23:    return "Chinese";
        case 24:    return "Mongolian";
        case 25:    return "Korean";
        case 26:    return "Siberian";
        case 27:    return "Siberian";
        case 28:    return "Japanese";
        case 29:    return "Uyghur";
        default:    return "INVALID_CULTURE";
    }
}

gs::Culture::Culture( const int groupId )
    :   groupId( groupId ),
        name( GenerateCultureName( groupId ) ),
        color()
{
}

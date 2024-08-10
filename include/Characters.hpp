#ifndef __CHARACTERS_HPP__
#define __CHARACTERS_HPP__

#include <cstdint>
#include <optional>

namespace CPPVersus {

enum Character {
    SAMURAI_JACK,
    AGENT_SMITH,
    JOKER,
    JASON,
    BANANA_GUARD,
    MARVIN_THE_MARTIAN,
    GIZMO,
    BLACK_ADAM,
    STRIPE,
    RICK,
    MORTY,
    LEBRON,
    THE_IRON_GIANT,
    TAZ,
    VELMA,
    ARYA_STARK,
    BATMAN,
    BUGS_BUNNY,
    FINN_THE_HUMAN,
    GARNET,
    HARLEY_QUINN,
    JAKE_THE_DOG,
    REINDOG,
    SHAGGY,
    STEVEN_UNIVERSE,
    SUPERMAN,
    TOM_AND_JERRY,
    WONDER_WOMAN
};

inline std::optional<Character> characterFromName(std::string name) {
    if(name == "character_jake") return std::optional<Character>(Character::JAKE_THE_DOG);

    return std::optional<Character>();
}

};

#endif
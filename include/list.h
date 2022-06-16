#pragma once

#include "utils/cstd.h"
#include "forward_declaration.h"
#include "player.h"
#include "card.h"

void free_list(List *game);
void build_list(List *game,Player bot);

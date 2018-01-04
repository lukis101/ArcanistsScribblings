#ifndef ENTITY_H
#define ENTITY_H

#include "globals.h"

// To create new entity, do:
// object_type name;
// name = CreateEntity(name);

template <typename T>
std::shared_ptr<T> CreateEntity(std::shared_ptr<T> data)
{
    EntityList->push_back(BaseClass(new T));
    std::shared_ptr<T> ent = std::dynamic_pointer_cast<T> (EntityList->back());
    ent->SetId( EntityIds );
    ent->SetSlot( EntityList->end()-EntityList->begin()-1 );
    EntityIds++;
    return ent;
};

template <typename T>
void DeleteEntity(T ent)
{
    EntityList->erase( EntityList->begin() + ent->GetSlot() );
};

#endif

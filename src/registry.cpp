#include "registry.hpp"
#include "engine.hpp"

namespace core {
EntityID Registry::create_entity() {
    if (!this->reuse_ids.empty()) {
        EntityID new_id = this->reuse_ids.front();
        this->reuse_ids.pop();
        return new_id;
    }

    return next_id++;
}

void Registry::destroy_entity(EntityID id) {
    // Remove from all types of component
    for (auto &pair : component_arrays) {
        pair.second->remove(id);
    }

    reuse_ids.push(id);
}

} // namespace core

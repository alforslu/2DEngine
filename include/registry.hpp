#pragma once

#include "engine.hpp"
#include "sparse.hpp"
#include <iostream>
#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>

namespace core {
class Registry {
  private:
    EntityID next_id = 0;
    std::queue<EntityID> reuse_ids;

    // Component sets
    std::unordered_map<std::type_index, std::unique_ptr<ISparseSet>>
        component_arrays;

  public:
    // TODO: Garbage collect entities without components?
    EntityID create_entity();
    void destroy_entity(EntityID id);

    // Component management templates
    template <typename T> void add_component(EntityID id, T component) {
        std::type_index type_index = typeid(T);

        // Check if type exists
        if (component_arrays.find(type_index) == component_arrays.end()) {
            component_arrays[type_index] = std::make_unique<SparseSet<T>>();
        }

        // Cast the specific component array to the matching type, so that we
        // can add to it
        auto array = static_cast<SparseSet<T> *>(
            this->component_arrays[type_index].get());

        array->insert(id, component);
    }

    template <typename T> void remove_component(EntityID id) {
        std::type_index type_index = typeid(T);

        if (component_arrays.find(type_index) != component_arrays.end()) {
            component_arrays[type_index]->remove(id);
        } else {
            std::cerr << "[ERROR] Failed to remove component from id: " << id
                      << ". Compnent does not exist on this id." << std::endl;
        }
    }
};
} // namespace core

#pragma once

#include "engine.hpp"
#include "sparse.hpp"
#include <cassert>
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
                      << ". Component does not exist on this id." << std::endl;
        }
    }

    template <typename T> SparseSet<T> *get_component_set() {
        std::type_index type_index = typeid(T);

        if (component_arrays.find(type_index) != component_arrays.end()) {
            return static_cast<SparseSet<T> *>(
                component_arrays[type_index].get());
        }
        return nullptr;
    }

    template <typename T> bool has_component(EntityID id) {
        std::type_index type_index = typeid(T);
        if (component_arrays.find(type_index) != component_arrays.end()) {
            auto components =
                static_cast<SparseSet<T> *>(component_arrays[type_index].get());
            return components->has(id);
        }
        return false;
    }

    template <typename T> T &get_component(EntityID id) {
        std::type_index type_index = typeid(T);

        assert(component_arrays.find(type_index) != component_arrays.end() &&
               "[ERROR] No such component is registered.");

        auto components =
            static_cast<SparseSet<T> *>(component_arrays[type_index].get());

        // Get handles component not existing
        return components->get(id);
    }

    // NOTE: Best practice is to provide types in order of smallest to largest
    // as this will mean the smallest loop
    template <typename T, typename... OtherTypes> std::vector<EntityID> view() {
        std::vector<EntityID> entities;

        auto primary_set = get_component_set<T>();
        if (!primary_set) {
            return entities;
        }

        auto &dense = primary_set->get_dense_array();
        for (int i = 0; i < dense.size(); i++) {
            EntityID id = primary_set->get_entity(i);
            if ((has_component<OtherTypes>(id) && ...)) {
                entities.push_back(id);
            }
        }
        return entities;
    }
};
} // namespace core

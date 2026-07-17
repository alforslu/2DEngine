#pragma once

#include "engine.hpp"
#include <cassert>
#include <vector>

namespace core {
const size_t BASE_ENTITIES = 10000; // Arbt, will dynamic resize
const size_t NO_INDEX = -1;

class ISparseSet {
  public:
    virtual ~ISparseSet() = default;
    virtual void remove(EntityID id) = 0;
};

template <typename T> class SparseSet : public ISparseSet {
  private:
    std::vector<T> dense;

    // Sparse matrix. Input an EntityID, and obtain the index of that entity in
    // the dense array.
    std::vector<size_t> entity_to_dense;
    std::vector<EntityID> dense_to_entity; // The opposite of above.

  public:
    SparseSet() { this->entity_to_dense.resize(BASE_ENTITIES, NO_INDEX); }

    void insert(EntityID id, T component) {
        // If the ID is greater than the sparse we need to resize
        // This is dangerous if a programmer manually sets
        // the ID to something large, since it will allocate
        // a bunch of empty memory up until that ID.
        if (id > this->entity_to_dense.size()) {
            this->entity_to_dense.resize(id + 128, NO_INDEX);
        }

        this->dense.push_back(component);
        this->entity_to_dense[id] = dense.size() - 1;
        this->dense_to_entity.push_back(id);
    }

    void remove(EntityID id) {
        // Swap target and last, and then remove the last
        size_t target_index = this->entity_to_dense[id];
        if (target_index == NO_INDEX) {
            return; // Do nothing, not here
        }

        size_t last_index = dense.size() - 1;
        if (target_index != last_index) {
            // Target is not last in dense, swap needed
            // This swaps the last entity to the deleted one
            // It then updates the two lookup tables to reflect this
            this->dense[target_index] = std::move(dense[last_index]);
            EntityID last_eid = this->dense_to_entity[last_index];
            this->dense_to_entity[target_index] = last_eid;
            this->entity_to_dense[last_eid] = target_index;
        }

        this->dense.pop_back();
        this->dense_to_entity.pop_back();
        this->entity_to_dense[id] = NO_INDEX;
    }

    T &get(EntityID id) {
        size_t index = this->entity_to_dense[id];
        assert (index != NO_INDEX && "[ERROR] Tried to get a component that does not exist.");
        return this->dense[index];
    }

    bool has(EntityID id) { 
        if (id >= this->entity_to_dense.size()) return false; // Don't index higher
        return this->entity_to_dense[id] != NO_INDEX; }

    // TODO: Should this be const return value?
    std::vector<T> &get_dense_array() { return this->dense; }

    EntityID get_entity(size_t dense_index) const {
        return this->dense_to_entity[dense_index];
    }
};
} // namespace core

#include <vector>
#include <list>
#include <utility>
#include <stdexcept>

// This class is an implemetation of a hash map with separate chaining. All
// elements are stored in a list(to provide linear iteration); the table has
// iterators to these elements, according to their hash. It dinamycally
// resizes itself, doubling the capacity, whenever the number of elements
// exceeds the current capacity
template<class Hash = std::hash<std::string>>
class StringSet {
  public:
    static constexpr size_t EXPANSION_COEFFICIENT = 2;

    using list_iterator =
      typename std::list<std::pair<std::string, size_t>>::iterator;
    using const_list_iterator =
      typename std::list<std::pair<std::string, size_t>>::const_iterator;
    
    class iterator {
    private:
        list_iterator ptr_;

    public:
        iterator(list_iterator ptr) : ptr_(ptr) {}

        std::string& operator*() {
            return ptr_->first;
        }

        iterator& operator++() {
            ++ptr_;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++ptr_;
            return temp;
        }

        bool operator==(const iterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    class const_iterator {
    private:
        const_list_iterator ptr_;

    public:
        const_iterator(const_list_iterator ptr) : ptr_(ptr) {}

        const std::string& operator*() {
            return ptr_->first;
        }

        const_iterator& operator++() {
            ++ptr_;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator temp = *this;
            ++ptr_;
            return temp;
        }

        bool operator==(const const_iterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
    };

    // Costructor used by default.
    StringSet(Hash hasher = Hash()) :
        nElements_(0),
        table_size_(1),
        hasher_(hasher) {
        table_ = std::vector<std::list<list_iterator>>(table_size_);
    }

    // Constructor by iterators.
    template<typename Iterator>
    StringSet(Iterator l, Iterator r, Hash hasher = Hash()) :
        nElements_(0),
        table_size_(1),
        hasher_(hasher) {
        table_ = std::vector<std::list<list_iterator>>(table_size_);
        while (l != r) {
            insert(*l++);
        }
    }

    // Costructor by std::initializer_list.
    StringSet(std::initializer_list<std::string> a,
            Hash hasher = Hash()) : StringSet(a.begin(), a.end(), hasher) {}

    // Returns the number of elements.
    size_t size() const {
        return nElements_;
    }

    // Checks whether hashmap is empty.
    bool empty() const {
        return nElements_ == 0;
    }

    // Returns the hash function.
    Hash hash_function() const {
        return hasher_;
    }

    // Inserts an element if it's not found.
    bool add(const std::string& data) {
        if (find(data) != end()) {
            return false;
        }
        size_t h = hasher_(data);
        size_t bucket = h % table_size_;
        ++nElements_;
        elements_.emplace_back(data, h);
        table_[bucket].push_back(--elements_.end());
        expand_if_necessary();
        return true;
    }

    // Erases an element.
    bool remove(const std::string& data) {
        size_t h = hasher_(data);
        size_t bucket = h % table_size_;
        for (auto i = table_[bucket].begin(); i != table_[bucket].end(); ++i) {
            if ((*i)->second == h) {
                elements_.erase(*i);
                table_[bucket].erase(i);
                --nElements_;
                return true;
            }
        }
        return false;
    }

    bool contains(const std::string& data) const {
        return find(data) != end();
    }

    // Returns begin interator.
    iterator begin() {
        return iterator{elements_.begin()};
    }

    // Returns end iterator.
    iterator end() {
        return iterator{elements_.end()};
    }

    // Returns const begin iterator
    const_iterator begin() const {
        return const_iterator{elements_.cbegin()};
    }

    // Returns const end iterator
    const_iterator end() const {
        return const_iterator{elements_.cend()};
    }

    // Finds an element and returns an iterator to it.
    iterator find(const std::string& data) {
        size_t h = hasher_(data);
        size_t bucket = h % table_size_;
        for (auto& iter : table_[bucket]) {
            if (iter->second == h) {
                return iterator{iter};
            }
        }
        return iterator{end()};
    }

    // Finds an element and returns a const iterator to it.
    const_iterator find(const std::string& data) const {
       size_t h = hasher_(data);
        size_t bucket = h % table_size_;
        for (const auto& iter : table_[bucket]) {
            if (iter->second == h) {
                return const_iterator{iter};
            }
        }
        return const_iterator{end()};
    }

    // Fully clears a hashmap.
    void clear() {
        nElements_ = 0;
        table_size_ = 1;
        elements_.clear();
        table_ = std::vector<std::list<list_iterator>>(table_size_);
    }

  private:
    std::vector<std::list<list_iterator>> table_;
    std::list<std::pair<std::string, size_t>> elements_;
    size_t nElements_;
    size_t table_size_;
    Hash hasher_;

    // Checks whether the load factor equals 1.
    bool need_to_expand() {
        return nElements_ > table_size_;
    }

    // Multiplies the table size by 2 and rebuilds it.
    void expand() {
        table_size_ *= EXPANSION_COEFFICIENT;
        table_ = std::vector<std::list<list_iterator>>(table_size_);
        for (auto i = elements_.begin(); i != elements_.end(); ++i) {
            size_t h = hasher_(i->first) % table_size_;
            table_[h].push_back(i);
        }
    }

    // Checks whether load factor equals 1. If so, multiplies table size by 2.
    void expand_if_necessary() {
        if (!need_to_expand()) {
            return;
        }
        expand();
    }
};

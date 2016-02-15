//
// Created by Michael Bühlmann on 15/02/16.
//

#ifndef CATANA_OBJECTCONTAINERSINK_HPP
#define CATANA_OBJECTCONTAINERSINK_HPP

class ObjectContainerSink : public Sink {
public:
    ObjectContainerSink(ObjectContainer& object_container) : object_container(object_container) {};
    long long int write(ObjectContainer::iterator read_iterator, size_t n) {return write_template(read_iterator, n);};
    long long int write(Object* read_iterator, size_t n) {return write_template(read_iterator, n);};

private:
    template<class ObjectIterator>
    long long int write_template(ObjectIterator read_iterator, size_t n) {
        for(size_t i=0; i<n; ++i) {
            object_container.push_back(*(read_iterator++));
        }
        return n;
    }
private:
    ObjectContainer& object_container;
};


#endif //CATANA_OBJECTCONTAINERSINK_HPP

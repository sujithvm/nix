// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SOURCE_H
#define NIX_SOURCE_H

#include <limits>
#include <functional>

#include <nix/util/filter.hpp>
#include <nix/util/util.hpp>
#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/ISource.hpp>

#include <nix/Platform.hpp>

namespace nix {


class NIXAPI Source : virtual public base::ISource, public base::EntityWithMetadata<base::ISource> {
    
public:


    Source();


    Source(const Source &other);


    Source(const std::shared_ptr<base::ISource> &p_impl);


    Source(std::shared_ptr<base::ISource> &&ptr);

    //--------------------------------------------------
    // Methods concerning child sources
    //--------------------------------------------------

    /**
     * Checks if this source has a specific source as direct descendant.
     *
     * @param id        The id of the source.
     *
     * @return True if a source with the given id is a direct descendant, false
     *         otherwise.
     */
    bool hasSource(const std::string &id) const {
        return backend()->hasSource(id);
    }

    /**
     * Checks if this source has a specific source as direct descendant.
     *
     * @param source         The Source.
     *
     * @return True if a source with the given id is a direct descendant, false
     *         otherwise.
     */
    bool hasSource(const Source &source) const {
        if(source == none) {
            throw std::runtime_error("Source::hasSource: emtpy Source entity given!");
        }
        return backend()->hasSource(source.id());
    }

    /**
     * Retrieves a specific child source that is a direct descendant.
     *
     * @param id        The id of the source.
     *
     * @return The source with the given id. If it doesn't exist an exception
     *         will be thrown.
     */
    Source getSource(const std::string &id) const {
        return backend()->getSource(id);
    }

    /**
     * Retrieves a specific source by index.
     *
     * @param index     The index of the source.
     *
     * @return The source at the specified index.
     */
    // TODO do really need this method?
    //      If not, check for similar methods in other classes.
    Source getSource(size_t index) const {
        return backend()->getSource(index);
    }

    /**
     * Returns the number of sources that are direct descendants of this source.
     *
     * @return The number of direct child sources.
     */
    size_t sourceCount() const {
        return backend()->sourceCount();
    }

    /**
     * Get sources associated with this source (aka "child" sources).
     *
     * The parameter "filter" is defaulted to giving back all sources.
     * To use your own filter pass a lambda that accepts a "Source"
     * as parameter and returns a bool telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object sources as a vector
     */
    std::vector<Source> sources(util::AcceptAll<Source>::type filter
                                = util::AcceptAll<Source>()) const
    {
        auto f = [this] (size_t i) { return getSource(i); };
        return getEntities<Source>(f,
                                   sourceCount(),
                                   filter);
    }


    /**
     * Go through the tree of sources originating from this source until
     * a max. level of "max_depth" and check for each source whether
     * to return it depending on predicate function "filter".
     * Return resulting vector of sources or empty vector if none found.
     * 
     * @param object filter function of type {@link nix::util::Filter::type}
     * @param int maximum depth to search tree
     * @return object vector of sources
     */
    std::vector<Source> findSources(std::function<bool(Source)> filter = util::AcceptAll<Source>(),
                                    size_t max_depth = std::numeric_limits<size_t>::max()) const;


    /**
     * Create a new root source.
     *
     * @param name      The name of the source to create.
     * @param type      The type of the source.
     *
     * @return The created source object.
     */
    Source createSource(const std::string &name, const std::string &type) {
        return backend()->createSource(name, type);
    }

    /**
     * Delete a root source and all its child sources from
     * the source.
     *
     * @param id        The id of the source to delete.
     *
     * @return True if the source was deleted, false otherwise.
     */
    bool deleteSource(const std::string &id) {
        return backend()->deleteSource(id);
    }

    /**
     * Delete a root source and all its child sources from
     * the source.
     *
     * @param source        The Source to delete.
     *
     * @return True if the source was deleted, false otherwise.
     */
    bool deleteSource(const Source &source) {
        if (source == none) {
            throw std::runtime_error("Source::deleteSource: empty Source entity given!");
        }
        return backend()->deleteSource(source.id());
    }

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    virtual Source &operator=(none_t) {
        nullify();
        return *this;
    }

    /**
     * Output operator
     */
    friend std::ostream& operator<<(std::ostream &out, const Source &ent);


};


} // namespace nix

#endif // NIX_SOURCE_H

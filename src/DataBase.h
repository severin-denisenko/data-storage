//
// Created by Severin on 15.11.2022.
//

#ifndef DATA_STORAGE_DATABASE_H
#define DATA_STORAGE_DATABASE_H

#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <alpaca/alpaca.h>

#include "Logger.h"

class DataBase
{
public:
    /**
     * @details Creates database.
     * @param filename Name of the database on disk
     */
    explicit DataBase(const std::string &filename);

    /**
     * @details Puts value into database.
     * @param t_key key string (Array of bytes)
     * @param t_value value string (Array of bytes)
     */
    void Put(const std::string &t_key, const std::string &t_value);

    /**
     * @details Returns value by key from database.
     * @param t_key key string (Array of bytes)
     * @return
     */
    std::string Get(const std::string &t_key);

    /**
     * @details Removes value by key from database.
     * @param t_key key string (Array of bytes)
     */
    void Delete(const std::string &t_key);

    /**
     * @details Set Synchronous/Asynchronous Writes. By default is false.
     * @param t_synchronous
     */
    void Synchronous(bool t_synchronous);

    /**
     * @details Batch writes data.
     * @tparam iter_type Vector, String, ets.
     * @param values_begin values.begin()
     * @param values_end values.end()
     * @param keys_begin keys.begin()
     * @param keys_end keys.end()
     */
    template<class iter_type>
    void BachWrite(iter_type values_begin, iter_type values_end, iter_type keys_begin, iter_type keys_end)
    {
        leveldb::WriteBatch batch;

        typename std::iterator_traits<iter_type>::difference_type n = std::distance(values_begin, values_end);
        typename std::iterator_traits<iter_type>::difference_type t_n = std::distance(keys_begin, keys_end);

        if (n != t_n){
            LOG(FATAL) << "Iterators for values and keys have to be equal length." << std::endl;
        }

        for (typename std::iterator_traits<iter_type>::difference_type i = 0; i < n; ++i)
        {
            batch.Delete(keys_begin[i]);
            batch.Put(keys_begin[i], values_begin[i]);
        }

        m_status = m_db->Write(m_write_options, &batch);

        checkErrors();
    }

    /**
     * @details Iterates every element of database with lambda expression.
     * @param f Lambda [](const std::string& str){ ... }
     */
    void Iterate(const std::function<void(const std::string&)>& f);

    ~DataBase();

private:
    leveldb::DB *m_db{};
    leveldb::Options m_options;
    leveldb::Status m_status;
    leveldb::WriteOptions m_write_options;
    leveldb::ReadOptions m_read_options;

    void checkErrors();
};


#endif //DATA_STORAGE_DATABASE_H

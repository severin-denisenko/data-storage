//
// Created by Severin on 15.11.2022.
//

#include "DataBase.h"
#include <iostream>

DataBase::DataBase(const std::string &t_filename)
{
    m_options.create_if_missing = true;
    m_status = leveldb::DB::Open(m_options, t_filename, &m_db);

    checkErrors();
}

void DataBase::Put(const std::string &t_key, const std::string &t_value)
{
    m_status = m_db->Put(leveldb::WriteOptions(), t_key, t_value);

    checkErrors();
}

std::string DataBase::Get(const std::string &t_key)
{
    std::string t_value;
    m_status = m_db->Get(m_read_options, t_key, &t_value);

    checkErrors();

    return t_value;
}

DataBase::~DataBase()
{
    delete m_db;
}

void DataBase::checkErrors()
{
    if (!m_status.ok())
        std::cerr << m_status.ToString() << std::endl; // TODO Logging
}

void DataBase::Delete(const std::string &t_key)
{
    m_status = m_db->Delete(m_write_options, t_key);

    checkErrors();
}

void DataBase::Synchronous(bool t_synchronous)
{
    m_write_options.sync = t_synchronous;
}

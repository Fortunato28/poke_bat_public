#include <iostream>
#include <memory>

#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TTransportUtils.h>

#include "client_controller.h"
#include "common.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace poke_bat::middleware;

ClientController::ClientController(const std::string& server_address)
{
    shared_ptr<TSocket> socket(new TSocket(server_address, port_));
    //shared_ptr<TTransport> transport(new TFramedTransport(socket));
    transport_ = make_shared<TFramedTransport>(socket);
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport_));
    thrift_client_ = make_unique<PokServerClient>(protocol);

    transport_->open();
}

ClientController::~ClientController()
{
    transport_->close();
}

const std::string ClientController::getSavedPoksTable()
{

    string result;
    thrift_client_->getSavedPoksTable(result);

    return result;
}

const Pokemon ClientController::getSavedPokByPrivateID(const std::string& private_id)
{
    Pokemon result;
    thrift_client_->getSavedPokByPrivateID(result, private_id);

    return result;
}

string ClientController::savePokemon(const string& private_id, const Pokemon& c_pok, const string& comment)
{
    string result;
    thrift_client_->savePokemon(result, private_id, c_pok, comment);

    return result;
}

void ClientController::getConfig(std::string& _return) try
{
    thrift_client_->getConfig(_return);
}
catch (TException& tx)
{
    cout << "ERROR: " << tx.what() << endl;
}

void ClientController::startFight(FightData& _return, const string& pub_id, const Pokemon& clientPokemon) try
{
    thrift_client_->startFight(_return, pub_id, clientPokemon);
    fight_id_ = _return.fight_id;
}
catch (TException& tx)
{
    cout << "ERROR: " << tx.what() << endl;
}

void ClientController::punch(RoundResult& _return) try
{
    thrift_client_->punch(_return, fight_id_);
}
catch (TException& tx)
{
    cout << "ERROR: " << tx.what() << endl;
}

void ClientController::defend(RoundResult& _return) try
{
    thrift_client_->defend(_return, fight_id_);
}
catch (TException& tx)
{
    cout << "ERROR: " << tx.what() << endl;
}

void ClientController::useSkill(RoundResult& _return, const std::string& skillName) try
{
    thrift_client_->useSkill(_return, fight_id_, skillName);
}
catch (TException& tx)
{
    cout << "ERROR: " << tx.what() << endl;
}

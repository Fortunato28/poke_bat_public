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

ClientController::ClientController()
{
    shared_ptr<TSocket> socket(new TSocket(host_, port_));
    //shared_ptr<TTransport> transport(new TFramedTransport(socket));
    transport_ = make_shared<TFramedTransport>(socket);
    shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport_));
    thrift_client_ = make_unique<PokServerClient>(protocol);
}

ClientController::~ClientController()
{
}

void ClientController::getConfig(std::string& _return) try
{
    transport_->open();

    thrift_client_->getConfig(_return);

    transport_->close();
}
catch (TException& tx)
{
    cout << "ERROR: " << tx.what() << endl;
}

void ClientController::startFight(FightData& _return, const int64_t complexity, const Pokemon& clientPokemon) try
{
    transport_->open();

    thrift_client_->startFight(_return, complexity, clientPokemon);
    fight_id_ = _return.fight_id;

    transport_->close();
}
catch (TException& tx)
{
    cout << "ERROR: " << tx.what() << endl;
}

void ClientController::punch(RoundResult& _return) try
{
    transport_->open();

    thrift_client_->punch(_return, fight_id_);

    transport_->close();
}
catch (TException& tx)
{
    cout << "ERROR: " << tx.what() << endl;
}

void ClientController::defend(RoundResult& _return) try
{
    transport_->open();

    thrift_client_->defend(_return, fight_id_);

    transport_->close();
}
catch (TException& tx)
{
    cout << "ERROR: " << tx.what() << endl;
}

void ClientController::useSkill(RoundResult& _return, const std::string& skillName) try
{
    transport_->open();

    thrift_client_->useSkill(_return, fight_id_, skillName);

    transport_->close();
}
catch (TException& tx)
{
    cout << "ERROR: " << tx.what() << endl;
}

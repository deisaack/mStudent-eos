#!/usr/bin/env bash

#nodeos -e -p eosio --plugin eosio::producer_plugin --plugin eosio::history_plugin --plugin eosio::chain_api_plugin --plugin eosio::history_plugin --plugin eosio::history_api_plugin --plugin eosio::http_plugin --access-control-allow-origin=* --contracts-console --http-validate-host=false --filter-on='*' --data-dir=data --hard-replay-blockchain
nodeos -e -p eosio --plugin eosio::producer_plugin --plugin eosio::history_plugin --plugin eosio::chain_api_plugin --plugin eosio::history_plugin --plugin eosio::history_api_plugin --plugin eosio::http_plugin --access-control-allow-origin=* --contracts-console --http-validate-host=false --filter-on='*' --data-dir=data

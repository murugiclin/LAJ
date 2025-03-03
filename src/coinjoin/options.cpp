// Copyright (c) 2021-2023 The Lajcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <coinjoin/options.h>

#include <util/system.h>
#include <univalue.h>

#include <cassert>

CCoinJoinClientOptions* CCoinJoinClientOptions::_instance{nullptr};
std::once_flag CCoinJoinClientOptions::onceFlag;

CCoinJoinClientOptions& CCoinJoinClientOptions::Get()
{
    std::call_once(onceFlag, CCoinJoinClientOptions::Init);
    assert(CCoinJoinClientOptions::_instance);
    return *CCoinJoinClientOptions::_instance;
}

void CCoinJoinClientOptions::SetEnabled(bool fEnabled)
{
    CCoinJoinClientOptions& options = CCoinJoinClientOptions::Get();
    options.fEnableCoinJoin = fEnabled;
}

void CCoinJoinClientOptions::SetMultiSessionEnabled(bool fEnabled)
{
    CCoinJoinClientOptions& options = CCoinJoinClientOptions::Get();
    options.fCoinJoinMultiSession = fEnabled;
}

void CCoinJoinClientOptions::SetSessions(int sessions)
{
    CCoinJoinClientOptions& options = CCoinJoinClientOptions::Get();
    options.nCoinJoinSessions = sessions;
}

void CCoinJoinClientOptions::SetRounds(int nRounds)
{
    CCoinJoinClientOptions& options = CCoinJoinClientOptions::Get();
    options.nCoinJoinRounds = nRounds;
}

void CCoinJoinClientOptions::SetAmount(CAmount amount)
{
    CCoinJoinClientOptions& options = CCoinJoinClientOptions::Get();
    options.nCoinJoinAmount = amount;
}

void CCoinJoinClientOptions::SetDenomsGoal(int denoms_goal)
{
    CCoinJoinClientOptions& options = CCoinJoinClientOptions::Get();
    options.nCoinJoinDenomsGoal = denoms_goal;
}

void CCoinJoinClientOptions::SetDenomsHardCap(int denoms_hardcap)
{
    CCoinJoinClientOptions& options = CCoinJoinClientOptions::Get();
    options.nCoinJoinDenomsHardCap = denoms_hardcap;
}

void CCoinJoinClientOptions::Init()
{
    assert(!CCoinJoinClientOptions::_instance);
    static CCoinJoinClientOptions instance;
    instance.fCoinJoinMultiSession = gArgs.GetBoolArg("-coinjoinmultisession", DEFAULT_COINJOIN_MULTISESSION);
    instance.nCoinJoinSessions = std::min(std::max((int)gArgs.GetArg("-coinjoinsessions", DEFAULT_COINJOIN_SESSIONS), MIN_COINJOIN_SESSIONS), MAX_COINJOIN_SESSIONS);
    instance.nCoinJoinRounds = std::min(std::max((int)gArgs.GetArg("-coinjoinrounds", DEFAULT_COINJOIN_ROUNDS), MIN_COINJOIN_ROUNDS), MAX_COINJOIN_ROUNDS);
    instance.nCoinJoinAmount = std::min(std::max((int)gArgs.GetArg("-coinjoinamount", DEFAULT_COINJOIN_AMOUNT), MIN_COINJOIN_AMOUNT), MAX_COINJOIN_AMOUNT);
    instance.nCoinJoinDenomsGoal = std::min(std::max((int)gArgs.GetArg("-coinjoindenomsgoal", DEFAULT_COINJOIN_DENOMS_GOAL), MIN_COINJOIN_DENOMS_GOAL), MAX_COINJOIN_DENOMS_GOAL);
    instance.nCoinJoinDenomsHardCap = std::min(std::max((int)gArgs.GetArg("-coinjoindenomshardcap", DEFAULT_COINJOIN_DENOMS_HARDCAP), MIN_COINJOIN_DENOMS_HARDCAP), MAX_COINJOIN_DENOMS_HARDCAP);
    CCoinJoinClientOptions::_instance = &instance;
}

void CCoinJoinClientOptions::GetJsonInfo(UniValue& obj)
{
    assert(obj.isObject());
    const CCoinJoinClientOptions& options = CCoinJoinClientOptions::Get();
    obj.pushKV("enabled", options.fEnableCoinJoin);
    obj.pushKV("multisession", options.fCoinJoinMultiSession);
    obj.pushKV("max_sessions", options.nCoinJoinSessions);
    obj.pushKV("max_rounds", options.nCoinJoinRounds);
    obj.pushKV("max_amount", options.nCoinJoinAmount);
    obj.pushKV("denoms_goal", options.nCoinJoinDenomsGoal);
    obj.pushKV("denoms_hardcap", options.nCoinJoinDenomsHardCap);
}

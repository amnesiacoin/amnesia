// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2018 The Amnesia developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "masternode-helpers.h"
#include "init.h"
#include "main.h"
#include "masternodeman.h"
#include "activemasternode.h"
#include "masternode-payments.h"
#include "amount.h"
#include "swifttx.h"

// A helper object for signing messages from Masternodes
CMasternodeSigner masternodeSigner;

void ThreadMasternodePool()
{
    if (fLiteMode) return; //disable all Masternode related functionality

    // Make this thread recognisable
    RenameThread("amnesia-mnpool");

    unsigned int c = 0;

    while (true) {
        MilliSleep(1000);

        // try to sync from all available nodes, one step at a time
        masternodeSync.Process();

        if (masternodeSync.IsBlockchainSynced()) {
            c++;

            // check if we should activate or ping every few minutes,
            // start right after sync is considered to be done
            if (c % MASTERNODE_PING_SECONDS == 0) activeMasternode.ManageStatus();

            if (c % 60 == 0) {
                mnodeman.CheckAndRemove();
                mnodeman.ProcessMasternodeConnections();
                masternodePayments.CleanPaymentList();
                CleanTransactionLocksList();
            }
        }
    }
}

bool CMasternodeSigner::IsVinAssociatedWithPubkey(CTxIn& vin, CPubKey& pubkey)
{
    CScript payee2;
    payee2 = GetScriptForDestination(pubkey.GetID());

    CAmount collateral = 10000 * COIN;

    CTransaction txVin;
    uint256 hash;
    if (GetTransaction(vin.prevout.hash, txVin, hash, true)) {
        BlockMap::iterator iter = mapBlockIndex.find(hash);
        if (iter != mapBlockIndex.end()) {
            int txnheight = iter->second->nHeight;
            if (txnheight <= GetSporkValue(SPORK_19_3000_COLLATERAL_BLOCK)) {
                collateral = 3000 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_20_4000_COLLATERAL_BLOCK)) {
                collateral = 4000 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_21_4200_COLLATERAL_BLOCK)) {
                collateral = 4200 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_22_4550_COLLATERAL_BLOCK)) {
                collateral = 4550 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_23_4750_COLLATERAL_BLOCK)) {
                collateral = 4750 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_24_4950_COLLATERAL_BLOCK)) {
                collateral = 4950 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_25_5150_COLLATERAL_BLOCK)) {
                collateral = 5150 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_26_5350_COLLATERAL_BLOCK)) {
                collateral = 5350 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_27_5600_COLLATERAL_BLOCK)) {
                collateral = 5600 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_28_5850_COLLATERAL_BLOCK)) {
                collateral = 5850 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_29_6150_COLLATERAL_BLOCK)) {
                collateral = 6150 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_30_6400_COLLATERAL_BLOCK)) {
                collateral = 6400 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_31_6750_COLLATERAL_BLOCK)) {
                collateral = 6750 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_32_7050_COLLATERAL_BLOCK)) {
                collateral = 7050 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_33_7400_COLLATERAL_BLOCK)) {
                collateral = 7400 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_34_7800_COLLATERAL_BLOCK)) {
                collateral = 7800 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_35_8200_COLLATERAL_BLOCK)) {
                collateral = 8200 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_36_8600_COLLATERAL_BLOCK)) {
                collateral = 8600 * COIN;
            } else if (txnheight <= GetSporkValue(SPORK_37_9200_COLLATERAL_BLOCK)) {
                collateral = 9200 * COIN;
            }
        }

        BOOST_FOREACH (CTxOut out, txVin.vout) {
            if (out.nValue == collateral) {
                if (out.scriptPubKey == payee2) return true;
            }
        }
    }

    return false;
}

bool CMasternodeSigner::SetKey(std::string strSecret, std::string& errorMessage, CKey& key, CPubKey& pubkey)
{
    CBitcoinSecret vchSecret;
    bool fGood = vchSecret.SetString(strSecret);

    if (!fGood) {
        errorMessage = _("Invalid private key.");
        return false;
    }

    key = vchSecret.GetKey();
    pubkey = key.GetPubKey();

    return true;
}

bool CMasternodeSigner::GetKeysFromSecret(std::string strSecret, CKey& keyRet, CPubKey& pubkeyRet)
{
    CBitcoinSecret vchSecret;

    if (!vchSecret.SetString(strSecret)) return false;

    keyRet = vchSecret.GetKey();
    pubkeyRet = keyRet.GetPubKey();

    return true;
}

bool CMasternodeSigner::SignMessage(std::string strMessage, std::string& errorMessage, vector<unsigned char>& vchSig, CKey key)
{
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << strMessage;

    if (!key.SignCompact(ss.GetHash(), vchSig)) {
        errorMessage = _("Signing failed.");
        return false;
    }

    return true;
}

bool CMasternodeSigner::VerifyMessage(CPubKey pubkey, vector<unsigned char>& vchSig, std::string strMessage, std::string& errorMessage)
{
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << strMessage;

    CPubKey pubkey2;
    if (!pubkey2.RecoverCompact(ss.GetHash(), vchSig)) {
        errorMessage = _("Error recovering public key.");
        return false;
    }

    if (fDebug && pubkey2.GetID() != pubkey.GetID())
        LogPrintf("CMasternodeSigner::VerifyMessage -- keys don't match: %s %s\n", pubkey2.GetID().ToString(), pubkey.GetID().ToString());

    return (pubkey2.GetID() == pubkey.GetID());
}

bool CMasternodeSigner::SetCollateralAddress(std::string strAddress)
{
    CBitcoinAddress address;
    if (!address.SetString(strAddress)) {
        LogPrintf("CMasternodeSigner::SetCollateralAddress - Invalid collateral address\n");
        return false;
    }
    collateralPubKey = GetScriptForDestination(address.Get());
    return true;
}

bool CMasternodeSigner::IsCollateralAmount(const CAmount& amount)
{
    return
            amount == 3000  * COIN ||
            amount == 4000  * COIN ||
            amount == 4200  * COIN ||
            amount == 4550  * COIN ||
            amount == 4750  * COIN ||
            amount == 4950  * COIN ||
            amount == 5150  * COIN ||
            amount == 5350  * COIN ||
            amount == 5600  * COIN ||
            amount == 5850  * COIN ||
            amount == 6150  * COIN ||
            amount == 6400  * COIN ||
            amount == 6750  * COIN ||
            amount == 7050  * COIN ||
            amount == 7400  * COIN ||
            amount == 7800  * COIN ||
            amount == 8200  * COIN ||
            amount == 8600  * COIN ||
            amount == 9200  * COIN ||
            amount == 10000 * COIN;
}

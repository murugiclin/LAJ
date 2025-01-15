// Copyright (c) 2011-2020 The Bitcoin Core developers
// Copyright (c) 2014-2024 The Lajcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_GUICONSTANTS_H
#define BITCOIN_QT_GUICONSTANTS_H

#include <chrono>
#include <cstdint>

using namespace std::chrono_literals;

/* A delay between model updates */
static constexpr auto MODEL_UPDATE_DELAY{250ms};

/* A delay between shutdown pollings */
static constexpr auto SHUTDOWN_POLLING_DELAY{200ms};

/* AskPassphraseDialog -- Maximum passphrase length */
static const int MAX_PASSPHRASE_SIZE = 1024;

/* LajcoinGUI -- Size of icons in status bar */
static const int STATUSBAR_ICONSIZE = 18;

/* LajcoinGUI -- Size of button icons e.g. in SendCoinEntry or SignVerifyMessageDialog */
static const int BUTTON_ICONSIZE = 23;

static const bool DEFAULT_SPLASHSCREEN = true;

/** Defines the half in RGB space, basically a grey in the middle between black and white */
#define RGB_HALF 0x7f7f7f
/** Path to the icon resource folder */
#define ICONS_PATH ":icons/"

/* Tooltips longer than this (in characters) are converted into rich text,
   so that they can be word-wrapped.
 */
static const int TOOLTIP_WRAP_THRESHOLD = 80;

/* Number of frames in spinner animation */
#define SPINNER_FRAMES 90

#define QAPP_ORG_NAME "Lajcoin"
#define QAPP_ORG_DOMAIN "dash.org"
#define QAPP_APP_NAME_DEFAULT "Lajcoin-Qt"
#define QAPP_APP_NAME_TESTNET "Lajcoin-Qt-testnet"
#define QAPP_APP_NAME_DEVNET "Lajcoin-Qt-%s"
#define QAPP_APP_NAME_REGTEST "Lajcoin-Qt-regtest"

/* One gigabyte (GB) in bytes */
static constexpr uint64_t GB_BYTES{1000000000};

// Default prune target displayed in GUI.
static constexpr int DEFAULT_PRUNE_TARGET_GB{2};

#endif // BITCOIN_QT_GUICONSTANTS_H

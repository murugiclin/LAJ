#!/usr/bin/env python3
# Copyright (c) 2024 The Lajcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Test that commands submitted by the platform user are filtered."""

from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import str_to_b64str, assert_equal

import http.client
import json
import os
import urllib.parse


class HTTPBasicsTest(BitcoinTestFramework):
    def set_test_params(self):
        self.num_nodes = 1
        self.supports_cli = False

    def setup_nodes(self):
        self.add_nodes(self.num_nodes)
        self.start_nodes()

    def setup_chain(self):
        super().setup_chain()
        # Append rpcauth to dash.conf before initialization
        rpcauthplatform = "rpcauth=platform-user:dd88fd676186f48553775d6fb5a2d344$bc1f7898698ead19c6ec7ff47055622dd7101478f1ff6444103d3dc03cd77c13"
        # rpcuser : platform-user
        # rpcpassword : password123
        rpcauthoperator = "rpcauth=operator:e9b45dd0b61a7be72155535435365a3a$8fb7470bc6f74d8ceaf9a23f49b06127723bd563b3ed5d9cea776ef01803d191"
        # rpcuser : operator
        # rpcpassword : otherpassword

        masternodeblskey="masternodeblsprivkey=58af6e39bb4d86b22bda1a02b134c2f5b71caffa1377540b02f7f1ad122f59e0"

        with open(os.path.join(self.options.tmpdir+"/node0", "dash.conf"), 'a', encoding='utf8') as f:
            f.write(masternodeblskey+"\n")
            f.write(rpcauthplatform+"\n")
            f.write(rpcauthoperator+"\n")

    def run_test(self):
        url = urllib.parse.urlparse(self.nodes[0].url)

        def send_command(method, params, auth, expected_status, should_not_match=False):
            conn = http.client.HTTPConnection(url.hostname, url.port)
            conn.connect()
            body = {"method": method}
            if len(params):
                body["params"] = params
            conn.request('POST', '/', json.dumps(body), {"Authorization": "Basic " + str_to_b64str(auth)})
            resp = conn.getresponse()
            if should_not_match:
                assert resp.status != expected_status
            else:
                assert_equal(resp.status, expected_status)
            conn.close()

        rpcuser_authpair_platform = "platform-user:password123"
        rpcuser_authpair_operator = "operator:otherpassword"
        rpcuser_authpair_wrong = "platform-user:rpcpasswordwrong"

        external_log_str = "HTTP: Calling handler for external user"
        always_expected_log_str = "ThreadRPCServer method="

        self.log.info('Try using a incorrect password for platform-user...')
        send_command("getbestblockhash", [], rpcuser_authpair_wrong, 401)

        self.log.info("Check that there's no external queue by default")
        with self.nodes[0].assert_debug_log(expected_msgs=[always_expected_log_str], unexpected_msgs = [external_log_str]):
            send_command("getbestblockhash", [], rpcuser_authpair_platform, 200)
        with self.nodes[0].assert_debug_log(expected_msgs=[always_expected_log_str], unexpected_msgs = [external_log_str]):
            send_command("getbestblockhash", [], rpcuser_authpair_operator, 200)

        self.log.info("Restart node with -rpcexternaluser")
        self.restart_node(0, extra_args=["-rpcexternaluser=platform-user"])

        with self.nodes[0].assert_debug_log(expected_msgs=[always_expected_log_str, external_log_str]):
            send_command("getbestblockhash", [], rpcuser_authpair_platform, 200)
        with self.nodes[0].assert_debug_log(expected_msgs=[always_expected_log_str], unexpected_msgs = [external_log_str]):
            send_command("getbestblockhash", [], rpcuser_authpair_operator, 200)

        self.log.info("Restart node with multiple external users")
        self.restart_node(0, extra_args=["-rpcexternaluser=platform-user,operator"])
        with self.nodes[0].assert_debug_log(expected_msgs=[always_expected_log_str, external_log_str]):
            send_command("getbestblockhash", [], rpcuser_authpair_platform, 200)
        with self.nodes[0].assert_debug_log(expected_msgs=[always_expected_log_str, external_log_str]):
            send_command("getbestblockhash", [], rpcuser_authpair_operator, 200)


if __name__ == '__main__':
    HTTPBasicsTest().main()

Amnesia - Forget the rest
=================================================

Amnesia - Forget the rest. In reality, where each coin with PoS Mining copies one another, it's really easy to forget them all when something new appears. Amnesia will bring freshness to the already well-known game. Flexible and easy-to-use crypto currency will provide unprecedented opportunities for investors. It is well known that existing crypto-currencies with a fixed reward for the block inevitably generate inflation that lead to the coin's price dump. Growth is observed only for a short time, due to a massive influx of initial investors and, often, ends with a complete depreciation of the asset. Forget about it. In Amnesia, the reward will depend on the number of active nodes in the network, it will vary flexibly - ensuring stability, stability of the exchange rate. Attract new investors.

License
-------

Amnesia Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is meant to be stable. Development is normally done in separate branches.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](/doc/unit-tests.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`

There are also [regression and integration tests](/qa) of the RPC interface, written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/qa) are installed) with: `qa/pull-tester/rpc-tests.py`

The Travis CI system makes sure that every pull request is built for Windows
and Linux, OS X, and that unit and sanity tests are automatically run.

### Manual Quality Assurance (QA) Testing

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.
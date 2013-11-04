#ifndef _BITCOIN_HPP_
#define _BITCOIN_HPP_
#include <boost/asio.hpp>
#include <boost/filesystem/path.hpp>

#include <list>

namespace bitcoin {
    namespace detail { class client; }

struct address_info
{
    bool isvalid;
    std::string address;
    bool ismine;
    std::string account;
};

struct server_info
{
    uint64_t     version;
    uint64_t     balance;
    uint64_t     blocks;
    uint64_t     connections;
    std::string  proxy;
    bool         generate;
    int32_t      genproclimit;
    double       difficulty;
    double       hashespersec;
    bool         testnet;
    uint64_t     keypoololdest;
    uint64_t     paytxfee;
    std::string  errors;
};

// References:
// * BIP-0022, Transactions Object Format (https://en.bitcoin.it/wiki/BIP_0022#Transactions_Object_Format)
// * bitcoind v0.8.5
struct transaction
{
    std::string data;
    std::string hash;
    std::list<uint64_t> depends;    // 1-based indices of transactions we depend on
    uint64_t fee;
    uint64_t sigops;
    bool required;  // non-required, not implemented in bitcoind, default value = false
};

struct coinbaseaux
{
    std::string flags;
};

// References:
// * BIP-0022, Block Template Request (https://en.bitcoin.it/wiki/BIP_0022#Block_Template_Request)
// * bitcoind v0.8.5
struct block_template
{
    uint64_t version;
    std::string previousblockhash;  // hex
    std::list<transaction> transactions;
    struct coinbaseaux coinbaseaux;  // non-required, implemented in bitcoind
    // transaction coinbasetxn; // not implemented in bitcoind
    uint64_t coinbasevalue;
    std::string target; // hex
    uint64_t mintime;
    std::list<std::string> mutable_;   // not in BIP-0022, in BIP-0023, implemented in bitcoind
    std::string noncerange; // hex
    uint64_t sigoplimit;  // non-required, implemented in bitcoind
    uint64_t sizelimit;   // non-required, implemented in bitcoind
    uint64_t curtime;
    std::string bits;   // hex
    uint64_t height;
};


class client
{
    public:
        client( boost::asio::io_service& ios );
        ~client();

        bool                      connect( const std::string& host, const std::string& port,
                                           const std::string& user, const std::string& pass );
        bool                      connect( const std::string& host_port,
                                           const std::string& user, const std::string& pass );
                                  
        std::string               backupwallet( const boost::filesystem::path& destination );
        std::string               getaccount( const std::string& address );
        std::string               getaccountaddress( const std::string& account );
        std::vector<std::string>  getaddressesbyaccount( const std::string& account );
        uint64_t                  getbalance( const std::string& account = "", uint32_t minconf = 1 );

        //getblockbycount( uint32_t height );
        uint32_t                  getblockcount();
        uint32_t                  getconnectioncount();
        double                    getdifficulty();
        bool                      getgenerate();
        server_info               getinfo();
                                  
        uint64_t                  getreceivedbyaddress( const std::string& address, uint32_t minconf = 1 );
        uint64_t                  getreceivedbyaccount( const std::string& account, uint32_t minconf = 1 );




        std::string  getnewaddress( const std::string& account = "" );
        void         setaccount( const std::string& address, const std::string& account );
        address_info validateaddress( const std::string& address );


        block_template getblocktemplate(const std::list<std::string>& capabilities,
                                        const std::string& mode = "template");
        bool submitblock(const std::string& block, std::string& out_reject_reason);

    private:
        detail::client* my;

        // create address... 
        // query balance...
};


}

#endif

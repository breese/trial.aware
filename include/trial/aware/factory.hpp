#ifndef TRIAL_AWARE_FACTORY_HPP
#define TRIAL_AWARE_FACTORY_HPP

#include <memory>
#include <boost/asio/io_service.hpp>
#include <trial/aware/announce_socket.hpp>
#include <trial/aware/monitor_socket.hpp>

namespace trial
{
namespace aware
{

class factory
{
public:
    virtual ~factory() = default;

    virtual std::unique_ptr<announce_socket> make_announce(boost::asio::io_service&) = 0;
    virtual std::unique_ptr<monitor_socket> make_monitor(boost::asio::io_service&) = 0;
};

} // namespace aware
} // namespace trial

#endif // TRIAL_AWARE_FACTORY_HPP

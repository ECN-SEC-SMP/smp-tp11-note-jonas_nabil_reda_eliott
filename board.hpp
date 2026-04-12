// Board.hpp
#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include "Models.hpp"

class Board {
private:
    std::vector<Route> routes;
    std::vector<Ticket> allTickets;

public:
    Board();
    void loadRoutesFromCSV(const std::string& filename);
    void loadTicketsFromCSV(const std::string& filename);

    std::vector<Route>&       getRoutes();
    const std::vector<Route>& getRoutes() const;
    std::vector<Ticket>&      getAllTickets();

    bool claimRoute(const std::string& cityA, const std::string& cityB, int playerId);
    void afficherRoutesPrises() const;
};

#endif
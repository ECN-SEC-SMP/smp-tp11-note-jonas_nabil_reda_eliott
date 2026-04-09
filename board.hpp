// Board.hpp
#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include "Models.hpp"
#include "carteTrain.hpp"


class Board {
private:
    std::vector<Route> routes;
    std::vector<Ticket> allTickets;
    std::vector<CarteTrain*> deck; 

    std::vector<Ticket> ticketDeck;

public:
    Board();
    void loadRoutesFromCSV(const std::string& filename);
    void loadTicketsFromCSV(const std::string& filename);
    void initDeck();          // ← nouveau
    CarteTrain* piocherCarte(); // ← nouveau
    std::vector<Route>& getRoutes();
    std::vector<Ticket>& getAllTickets();
    bool claimRoute(const std::string& cityA, const std::string& cityB, int playerId);

    void initTicketDeck();
    Ticket piocherTicket();
    bool ticketDeckVide() const;
};
#endif
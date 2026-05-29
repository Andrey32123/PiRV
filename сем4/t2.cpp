// task2
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

class Account {
public:
    double balance;
    std::mutex mtx;
    std::condition_variable cv;

    Account(double b) : balance(b) {}

    double get_balance() { return balance; }

    void deposit(double amount) {
        balance += amount;
    }

    bool withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            return true;
        }
        return false;
    }
};

class Bank {
private:
    std::vector<Account*> accounts;
    std::mutex cout_mutex;

public:
    Bank(const std::vector<double>& balances) {
        for (double b : balances) {
            accounts.push_back(new Account(b));
        }
    }

    ~Bank() {
        for (auto acc : accounts) delete acc;
    }

    inline bool transfer(int from, int to, double amount) {
        if (from == to || amount <= 0) return false;
        if (from < 0 || from >= (int)accounts.size() || 
            to < 0 || to >= (int)accounts.size()) return false;

        auto id = std::this_thread::get_id();

        Account* acc_from = accounts[from];
        Account* acc_to = accounts[to];

        std::unique_lock<std::mutex> lock1(acc_from->mtx, std::defer_lock);
        std::unique_lock<std::mutex> lock2(acc_to->mtx, std::defer_lock);

        if (from < to) {
            lock1.lock();
            lock2.lock();
        } else {
            lock2.lock();
            lock1.lock();
        }

        while (acc_from->balance < amount) {
            std::cout << "Thread " << id << " waiting: insufficient funds from account " 
                      << from << " (balance: " << acc_from->balance << ")" << std::endl;
            acc_from->cv.wait(lock1);
        }

        acc_from->balance -= amount;
        acc_to->balance += amount;

        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Thread " << id << " transferred " << amount
                      << " from " << from << " to " << to 
                      << " (from bal: " << acc_from->balance 
                      << ", to bal: " << acc_to->balance << ")" << std::endl;
        }

        acc_to->cv.notify_all();
        return true;
    }

    double get_total_balance() {
        double total = 0;
        for (auto acc : accounts) {
            total += acc->get_balance();
        }
        return total;
    }

    void print_balances() {
        for (size_t i = 0; i < accounts.size(); i++) {
            std::cout << "Account " << i << ": " << accounts[i]->get_balance() << std::endl;
        }
    }

    size_t size() const { return accounts.size(); }
};

void client_work(Bank& bank, int client_id, int num_transfers) {
    {
        std::lock_guard<std::mutex> lock(std::cout_mutex);
        std::cout << "Client " << client_id << " (thread " 
                  << std::this_thread::get_id() << ") started" << std::endl;
    }

    for (int i = 0; i < num_transfers; i++) {
        int from = rand() % bank.size();
        int to = rand() % bank.size();
        double amount = (rand() % 25) + 5;

        bank.transfer(from, to, amount);
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    {
        std::lock_guard<std::mutex> lock(std::cout_mutex);
        std::cout << "Client " << client_id << " finished" << std::endl;
    }
}

std::mutex std::cout_mutex;

int main() {
    srand(time(nullptr));

    std::vector<double> balances = { 1200, 800, 2500, 1800, 3000 };
    Bank bank(balances);

    double initial_total = bank.get_total_balance();
    std::cout << "Initial balances:" << std::endl;
    bank.print_balances();
    std::cout << "Initial total: " << initial_total << std::endl << std::endl;

    std::vector<std::thread> threads;
    int num_clients = 4;
    int transfers_per_client = 8;

    for (int i = 0; i < num_clients; i++) {
        threads.push_back(std::thread(client_work, std::ref(bank), i, transfers_per_client));
    }

    for (auto& t : threads) {
        t.join();
    }

    double final_total = bank.get_total_balance();
    std::cout << "\nFinal balances:" << std::endl;
    bank.print_balances();
    std::cout << "Initial total: " << initial_total << std::endl;
    std::cout << "Final total: " << final_total << std::endl;
    std::cout << "Sum preserved: " << (initial_total == final_total ? "YES" : "NO") << std::endl;

    return 0;
}
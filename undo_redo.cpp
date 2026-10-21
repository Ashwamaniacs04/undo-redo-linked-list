// Undo / Redo — Doubly Linked List
// DSA project (C++).
//   add    -> new node after current, redo branch is deleted
//   undo   -> current = current->prev
//   redo   -> current = current->next  (or head, if current is before the start)
//
// Build:  g++ -std=c++17 -Wall -Wextra -o undo_redo undo_redo.cpp
// Run:    ./undo_redo

#include <iostream>
#include <string>

struct Action {
    std::string command;
    int x;
    int y;
};

struct Node {
    Action action;
    Node* prev;
    Node* next;
    explicit Node(Action a) : action(std::move(a)), prev(nullptr), next(nullptr) {}
};

class UndoRedo {
public:
    UndoRedo() = default;
    ~UndoRedo() { destroyFrom(head); }

    UndoRedo(const UndoRedo&) = delete;
    UndoRedo& operator=(const UndoRedo&) = delete;

    void add(Action action) {
        Node* node = new Node(std::move(action));
        if (current) {
            destroyFrom(current->next);
            current->next = node;
            node->prev = current;
        } else {
            // Pointer is before the first node: the old chain is discarded.
            destroyFrom(head);
            head = node;
        }
        current = node;
    }

    bool undo() {
        if (!current) return false;
        current = current->prev;
        return true;
    }

    bool redo() {
        if (!current) {
            if (!head) return false;
            current = head;
            return true;
        }
        if (!current->next) return false;
        current = current->next;
        return true;
    }

    void print() const {
        if (!head) {
            std::cout << "(empty list)  current = null\n";
            return;
        }
        std::cout << "list: ";
        if (!current) std::cout << "[CURRENT] ";
        for (Node* n = head; n; n = n->next) {
            const bool here = n == current;
            std::cout << (here ? "[" : "(") << n->action.command << " "
                      << n->action.x << "," << n->action.y << (here ? "]" : ")");
            if (n->next) std::cout << " <-> ";
        }
        std::cout << "\n";
        if (!current) std::cout << "current sits before the first node (nothing applied)\n";
    }

private:
    Node* head = nullptr;
    Node* current = nullptr;

    static void destroyFrom(Node* start) {
        while (start) {
            Node* next = start->next;
            delete start;
            start = next;
        }
    }
};

static void menu() {
    std::cout << "\n1 Add action   2 Undo   3 Redo   4 Show list   5 Exit\n> ";
}

int main() {
    UndoRedo history;
    int choice = 0;
    while (true) {
        menu();
        if (!(std::cin >> choice)) break;
        if (choice == 5) break;
        if (choice == 1) {
            Action action;
            std::cout << "Command (place/erase): ";
            std::cin >> action.command;
            std::cout << "x y: ";
            std::cin >> action.x >> action.y;
            history.add(std::move(action));
            std::cout << "Added. Redo branch after current was cut.\n";
            history.print();
        } else if (choice == 2) {
            std::cout << (history.undo() ? "Undid. current moved to prev.\n"
                                         : "Nothing to undo.\n");
            history.print();
        } else if (choice == 3) {
            std::cout << (history.redo() ? "Redid. current moved to next.\n"
                                         : "Nothing to redo.\n");
            history.print();
        } else if (choice == 4) {
            history.print();
        } else {
            std::cout << "Pick 1-5.\n";
        }
    }
    return 0;
}

Does the AI think like Pong?
Does the AI know your player's previous actions and can calculate the possibility of something happening?
Pointers slide
Different colors for diff customer state, green for neutral, red for anger
Port everything to physics
Run hungry fsm, run not hungry fsm(solo hunt and group hunt fsm)
Decision tree, if my dir vec/proximity is a customer, go to him and take his order, if he is leaving hungry and angry, move twice as quickly to bring him back
Use bayesian rules - write in proposal
change your customer and waiter fsm

4 rules
A - Probability of going to customer who seems most urgent
B - Go to first customer then sequentially depending who came first
C - If customer is leaving, zao to him first
D - Closest proximity customer using getDist()
E - Customer is impatient
N - Number of customers

Use a decision tree

P(A) = P(C/E) P(D)
P(B) = P(D)

P(C) = 0.25
P(E) = 0.6
P(C/E) = 0.2
P(D) = 0.4
P(A) = 0.8

List of Features
- Markov and Cascade model multi fsm
- Map of vectors containing game objects like door and chef's table
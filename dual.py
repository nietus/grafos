import numpy as np

def initialize_problem():
    supply = np.array([25,25])
    demand = np.array([15,15,20])
    costs = np.array([[20,5,3],
                      [7,4,1]])

    # Step 0.1: Set A = (b1, b2, ..., bn, -a1, -a2, ..., -am)
    A = np.concatenate((demand, -supply))

    # Step 0.2: Set u_i = 0 and calculate v_j as the minimum cost in each column
    m, n = costs.shape
    U = np.zeros(m)                     # Set u_i = 0 for all origins
    V = np.min(costs, axis=0)          # Set v_j = min{C_{ij}, i = 1, ..., m} for all destinations

    # Step 0.3: Define the basic cell set
    basic_cell_set = []
    for j in range(n):
        i = np.argmin(costs[:, j]) + 1  # Find the origin with minimum cost for destination j (1-based index)
        basic_cell_set.append((i, j + 1))  # Add (i, j)
    for i in range(1, m + 1):
        basic_cell_set.append((i, 0))      # Add dummy cells

    # Generate D matrix
    D = np.zeros((m + n, m + n))

    # Rule 1: Set D_{ii} = 1 for i = 0 to n - 1
    D[0:n, 0:n] = np.eye(n)

    # Rule 2: For each basic cell (i, j), set D_{j-1, n + i - 1} = -1 (excluding virtual cells)
    for cell in basic_cell_set:
        i, j = cell
        if j > 0:  # Exclude virtual cells (j = 0)
            D[j - 1, n + i - 1] = -1  # Adjust indices for 0-based indexing

    # Rule 3: Set D_{ii} = -1 for i = n to n + m - 1
    D[n:, n:] = -np.eye(m)

    # Eliminate negative zeros (e.g., -0) by setting them to 0 explicitly
    D = np.where(np.isclose(D, 0), 0, D)

    # Compute initial objective
    psi = np.dot(U, supply) + np.dot(V, demand)

    # Initialize a flag for optimality
    is_optimal = False
    iteration = 0  # To keep track of iterations

    while not is_optimal:
        iteration += 1
        print(f"\n--- Iteration {iteration} ---")

        # Step 1: Determine Y = A * D
        Y = np.dot(A, D)
        print("Y (Vector):", Y)

        # Step 1.3: Check for optimality
        if np.all(Y >= 0):
            print("The solution is optimal.")
            print(f"Final Objective Value (ψ): {psi}")
            break  # Exit the loop as the solution is optimal

        # Find the leaving cell: The most negative element in Y
        leaving_cell_index = np.argmin(Y)
        leaving_cell_value = Y[leaving_cell_index]
        print(f"Leaving Cell Index: {leaving_cell_index}, Leaving Cell Value: {leaving_cell_value}")

        Q = D[:n, leaving_cell_index]  # First n elements of the k-th column
        P = D[n:, leaving_cell_index]  # Last m elements of the k-th column

        # Step 2.2: Calculate theta_ij for all non-basic cells
        thetas = []
        for i in range(m):
            for j in range(n):
                if (i + 1, j + 1) not in basic_cell_set:  # Non-basic cell
                    p_i = P[i]  # P value for row i
                    q_j = Q[j]  # Q value for column j
                    if p_i - q_j > 0:
                        # Corrected formula for theta_ij
                        theta_ij = costs[i, j] + U[i] - V[j]
                        thetas.append(((i + 1, j + 1), theta_ij))

        print("Thetas (Non-Basic Cells):", thetas)

        # Step 2.3: Find the entering cell
        if thetas:
            entering_cell = min(thetas, key=lambda x: x[1])  # Smallest theta
            entering_cell_pos, theta_st = entering_cell
            entering_row = entering_cell_pos[0] - 1  # Extract row index of entering cell (0-based index)
            entering_col = entering_cell_pos[1] - 1
            print(f"Entering Cell: {entering_cell_pos}, θ_st = {theta_st}")
        else:
            print("No entering cell found. Problem might be optimal or degenerate.")
            break  # Exit if no entering cell is found

        Dnovo = D.copy()

        # Step 3.1.1: For the elements of column k in D, Dnovo_lk = -D_lk
        Dnovo[:, leaving_cell_index] = -D[:, leaving_cell_index]

        # Step 3.1.2: Update Dnovo for other columns
        for r in range(D.shape[1]):
            if r != leaving_cell_index:
                for l in range(D.shape[0]):
                    Dnovo[l, r] = D[l, r] + (D[n + entering_row, r] - D[leaving_cell_index, r]) * Dnovo[l, leaving_cell_index]

        # Eliminate negative zeros (e.g., -0) by setting them to 0 explicitly
        Dnovo = np.where(np.isclose(Dnovo, 0), 0, Dnovo)

        print("Updated Dnovo Matrix:")
        print(Dnovo)

        # Step 3.2: Update the basic cell set
        # Identify the leaving cell's position in basic_cell_set
        leaving_cell = None
        for cell in basic_cell_set:
            if cell[1] == 0 and (cell[0] - 1) == leaving_cell_index:
                leaving_cell = cell
                break
        if leaving_cell is None:
            # If leaving cell is not a dummy, find it in basic_cell_set
            leaving_cell = basic_cell_set[leaving_cell_index]

        if leaving_cell in basic_cell_set:
            basic_cell_set.remove(leaving_cell)
        basic_cell_set.append(entering_cell_pos)  # Add the entering cell

        print("Updated Basic Cell Set:", basic_cell_set)

        # Step 3.3: Update the objective value
        theta_st = entering_cell[1]  # Theta value of the entering cell
        updated_U = U - theta_st * P  # Update u_i values
        updated_V = V - theta_st * Q  # Update v_j values

        # Compute the updated objective value
        updated_psi = np.dot(demand, updated_V) - np.dot(supply, updated_U)

        # Print the updated results
        print("Updated U (u_i):", updated_U)
        print("Updated V (v_j):", updated_V)
        print("Updated Objective Value (ψ):", updated_psi)

        # Update U and V for the next iteration
        U = updated_U
        V = updated_V
        psi = updated_psi

        # Update D for the next iteration
        D = Dnovo.copy()

    # Final results
    print("\n--- Final Results ---")
    print("Final Supply:", supply)
    print("Final Demand:", demand)
    print("Final Cost Matrix:")
    print(costs)
    print("Final A:", A)
    print("Final V:", V)
    print("Final U:", U)
    print("Final Basic Cell Set:", basic_cell_set)
    print("Final D Matrix:")
    print(D)
    print("Final Objective Value (ψ):", psi)

# Run the function to initialize and solve the transportation problem
initialize_problem()

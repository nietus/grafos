import numpy as np

def initialize_problem():
    supply = np.array([400, 300, 400])  # Supplies for origins O1, O2, O3
    demand = np.array([450, 350])  # Demands for destinations D1, D2
    costs = np.array([[3, 6],
                      [4, 5],
                      [7, 3]])  # Cost matrix for transportation

    # Step 0.1: Set A = (b1, b2, ..., bn, -a1, -a2, ..., -am)
    A = np.concatenate((demand, -supply))
    
    # Step 0.2: Set u_i = 0 and calculate v_j as the minimum cost in each column
    m, n = costs.shape
    U = np.zeros(m)  # Set u_i = 0 for all origins
    V = np.min(costs, axis=0)  # Set v_j = min{C_{ij}, i = 1, ..., m} for all destinations
    
    # Basic cell set
    basic_cell_set = [(1, 1), (3, 2), (1, 0), (2, 0), (3, 0)]  # Based on reference example

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
    
    # Compute objective
    psi = np.dot(U, supply) + np.dot(V, demand)
    
    # Step 1: Determine Y = A * D
    Y = np.dot(A, D)

    # Find the leaving cell: The most negative element in Y
    leaving_cell_index = np.argmin(Y)
    leaving_cell_value = Y[leaving_cell_index]
    
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
                    theta_ij = costs[i, j] - U[i] - V[j]
                    thetas.append(((i + 1, j + 1), theta_ij))
    
        
    # Print the results to verify
    print("Supply:", supply)
    print("Demand:", demand)
    print("Cost Matrix:")
    print(costs)
    print("A:", A)
    print("V:", V)
    print("U:", U)
    print("Basic Cell Set:", basic_cell_set)
    print("D Matrix:")
    print(D)
    print("ψ:", psi)
    print("Y (Vector):", Y)
    print("Leaving Cell Index:", leaving_cell_index)
    print("Leaving Cell Value:", leaving_cell_value)
    print("Thetas (Non-Basic Cells):", thetas)
    
    # Step 2.3: Find the entering cell
    if thetas:
        entering_cell = min(thetas, key=lambda x: x[1])  # Smallest theta
        print(f"Entering Cell: {entering_cell[0]}, θ_st = {entering_cell[1]}")
    else:
        print("No entering cell found. Problem is optimal.")
        
    # Step 3.1.1: Invert the sign of column k
    D[:, leaving_cell_index] *= -1

    # Eliminate negative zeros (e.g., -0) by setting them to 0 explicitly
    D = np.where(np.isclose(D, 0), 0, D)
    
    # Final Results
    print("\nUpdated D Matrix:")
    print(D)
    
# Run the function to initialize the problem
initialize_problem()
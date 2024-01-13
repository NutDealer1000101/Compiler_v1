<h1>Order of operations:</h1>
<ol>
  <li>Terminal: Integer Literal / Identifier</li>
  <li>Multiplicative: Mulitplication / Division</li>
  <li>Additive: Pluss / MInus)/li>
</ol>
<h1>Grammar:</h1>

$$
\begin{align}
	[\text{Program}] &\to [\text{Stmt}]^*
	\\
	\\
	[\text{Stmt}] &\to
	\begin{cases}
		[\text{Expr}]; \\
		[\text{IntDec}]; \\
		[\text{ReturnStmt}]; \\
	\end{cases}
	\\
	[\text{IntDec}] &\to
	\begin{cases}
		int\ \text{Identifier}\ = [\text{Expr}]
	\end{cases}
	\\
	[\text{ReturnStmt}] &\to
	\begin{cases}
		return\ [\text{Expr}]
	\end{cases}
	\\
	\\
	[\text{Expr}] &\to
	\begin{cases}
		[\text{Terminal}] \\
		[\text{BinaryExpr}] \\
	\end{cases}
	\\
	[\text{BinaryExpr}] &\to
	\begin{cases}
		[\text{Expr}] + [\text{Expr}] \\
		[\text{Expr}] - [\text{Expr}] \\
		[\text{Expr}] * [\text{Expr}] \\
	\end{cases}
	\\
	\\
	[\text{Terminal}] &\to
	\begin{cases}
		\text{Identifier} \\
		\text{IntLiteral} \\
	\end{cases}
\end{align}
$$

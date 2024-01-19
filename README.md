<h1>Order of operations:</h1>
<ol>
  <li>Terminal: Integer Literal / Identifier</li>
  <li>Multiplicative: Mulitplication / Division</li>
  <li>Additive: Pluss / Minus</li>
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
 		([\text{Expr}]) \\
	\end{cases}
\end{align}
$$

<h1>References:</h1>
<a href="https://www.youtube.com/playlist?list=PLUDlas_Zy_qC7c5tCgTMYq2idyyT241qs">Pixeled: Creating a Compiler playlist</a>
<br>
<a href="https://eli.thegreenplace.net/2012/08/02/parsing-expressions-by-precedence-climbing">The Green Place: Presedence Climbing</a>
<br>
<a href="https://en.wikipedia.org/wiki/Compiler">Wikipedia: Compiler</a>
<br>

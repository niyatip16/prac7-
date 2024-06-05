
#include "CompilerParser.h"

/**
 * Constructor for the CompilerParser
 * @param tokens A linked list of tokens to be parsed
 */
CompilerParser::CompilerParser(std::list<Token*> tokens) : tokens(tokens), currentToken(this->tokens.begin()) {}

/**
 * Generates a parse tree for a single program
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileProgram() {
    if (have("keyword", "class")) {
        return compileClass();
    } else {
        throw ParseException();
    }
}

// Generates a parse tree for a single class
ParseTree* CompilerParser::compileClass() {
    ParseTree* classNode = new ParseTree("class", "");

    mustBe("keyword", "class");
    classNode->addChild(new ParseTree("keyword", "class"));

    Token* className = mustBe("identifier", "");
    classNode->addChild(new ParseTree("identifier", className->getValue()));

    mustBe("symbol", "{");
    classNode->addChild(new ParseTree("symbol", "{"));

    while (have("keyword", "static") || have("keyword", "field")) {
        classNode->addChild(compileClassVarDec());
    }

    // Assuming there would be subroutine declarations (compileSubroutine) here
    // while (have("keyword", "constructor") || have("keyword", "function") || have("keyword", "method")) {
    //    classNode->addChild(compileSubroutine());
    //}

    mustBe("symbol", "}");
    classNode->addChild(new ParseTree("symbol", "}"));

    return classNode;
}

// Generates a parse tree for a static variable declaration or field declaration
ParseTree* CompilerParser::compileClassVarDec() {
    ParseTree* classVarDecNode = new ParseTree("classVarDec", "");

    if (have("keyword", "static") || have("keyword", "field")) {
        classVarDecNode->addChild(new ParseTree("keyword", current()->getValue()));
        next();
    } else {
        throw ParseException();
    }

    Token* type = current();
    if (have("keyword", "int") || have("keyword", "char") || have("keyword", "boolean") || type->getType() == "identifier") {
        classVarDecNode->addChild(new ParseTree(type->getType(), type->getValue()));
        next();
    } else {
        throw ParseException();
    }

    Token* varName = mustBe("identifier", "");
    classVarDecNode->addChild(new ParseTree("identifier", varName->getValue()));

    while (have("symbol", ",")) {
        next();
        classVarDecNode->addChild(new ParseTree("symbol", ","));
        Token* additionalVarName = mustBe("identifier", "");
        classVarDecNode->addChild(new ParseTree("identifier", additionalVarName->getValue()));
    }

    mustBe("symbol", ";");
    classVarDecNode->addChild(new ParseTree("symbol", ";"));

    return classVarDecNode;
}

/**
 * Generates a parse tree for a method, function, or constructor
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutine() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine's parameters
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileParameterList() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine's body
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileSubroutineBody() {
    return NULL;
}

/**
 * Generates a parse tree for a subroutine variable declaration
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileVarDec() {
    return NULL;
}

/**
 * Generates a parse tree for a series of statements
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileStatements() {
    ParseTree* statementsNode = new ParseTree("statements", "");

    while (have("keyword", "let") || have("keyword", "if") || have("keyword", "while") || have("keyword", "do") || have("keyword", "return")) {
        if (have("keyword", "let")) {
            statementsNode->addChild(compileLet());
        } else if (have("keyword", "if")) {
            statementsNode->addChild(compileIf());
        } else if (have("keyword", "while")) {
            statementsNode->addChild(compileWhile());
        } else if (have("keyword", "do")) {
            statementsNode->addChild(compileDo());
        } else if (have("keyword", "return")) {
            statementsNode->addChild(compileReturn());
        }
    }

    return statementsNode;
}


/**
 * Generates a parse tree for a let statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileLet() {
    ParseTree* letStatementNode = new ParseTree("letStatement", "");

    mustBe("keyword", "let");
    letStatementNode->addChild(new ParseTree("keyword", "let"));

    Token* varName = mustBe("identifier", "");
    letStatementNode->addChild(new ParseTree("identifier", varName->getValue()));

    mustBe("symbol", "=");
    letStatementNode->addChild(new ParseTree("symbol", "="));

    letStatementNode->addChild(compileExpression());

    mustBe("symbol", ";");
    letStatementNode->addChild(new ParseTree("symbol", ";"));

    return letStatementNode;
}

/**
 * Generates a parse tree for an if statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileIf() {
    ParseTree* ifStatementNode = new ParseTree("ifStatement", "");

    mustBe("keyword", "if");
    ifStatementNode->addChild(new ParseTree("keyword", "if"));

    mustBe("symbol", "(");
    ifStatementNode->addChild(new ParseTree("symbol", "("));

    ifStatementNode->addChild(compileExpression());

    mustBe("symbol", ")");
    ifStatementNode->addChild(new ParseTree("symbol", ")"));

    mustBe("symbol", "{");
    ifStatementNode->addChild(new ParseTree("symbol", "{"));

    ifStatementNode->addChild(compileStatements());

    mustBe("symbol", "}");
    ifStatementNode->addChild(new ParseTree("symbol", "}"));

    if (have("keyword", "else")) {
        mustBe("keyword", "else");
        ifStatementNode->addChild(new ParseTree("keyword", "else"));

        mustBe("symbol", "{");
        ifStatementNode->addChild(new ParseTree("symbol", "{"));

        ifStatementNode->addChild(compileStatements());

        mustBe("symbol", "}");
        ifStatementNode->addChild(new ParseTree("symbol", "}"));
    }

    return ifStatementNode;
}


/**
 * Generates a parse tree for a while statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileWhile() {
    ParseTree* whileStatementNode = new ParseTree("whileStatement", "");

    mustBe("keyword", "while");
    whileStatementNode->addChild(new ParseTree("keyword", "while"));

    mustBe("symbol", "(");
    whileStatementNode->addChild(new ParseTree("symbol", "("));

    whileStatementNode->addChild(compileExpression());

    mustBe("symbol", ")");
    whileStatementNode->addChild(new ParseTree("symbol", ")"));

    mustBe("symbol", "{");
    whileStatementNode->addChild(new ParseTree("symbol", "{"));

    whileStatementNode->addChild(compileStatements());

    mustBe("symbol", "}");
    whileStatementNode->addChild(new ParseTree("symbol", "}"));

    return whileStatementNode;
}

/**
 * Generates a parse tree for a do statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileDo() {
    ParseTree* doStatementNode = new ParseTree("doStatement", "");

    mustBe("keyword", "do");
    doStatementNode->addChild(new ParseTree("keyword", "do"));

    doStatementNode->addChild(compileExpression());

    mustBe("symbol", ";");
    doStatementNode->addChild(new ParseTree("symbol", ";"));

    return doStatementNode;
}

/**
 * Generates a parse tree for a return statement
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileReturn() {
    ParseTree* returnStatementNode = new ParseTree("returnStatement", "");

    mustBe("keyword", "return");
    returnStatementNode->addChild(new ParseTree("keyword", "return"));

    if (!have("symbol", ";")) {
        returnStatementNode->addChild(compileExpression());
    }

    mustBe("symbol", ";");
    returnStatementNode->addChild(new ParseTree("symbol", ";"));

    return returnStatementNode;
}

/**
 * Generates a parse tree for an expression
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpression() {
    ParseTree* expressionNode = new ParseTree("expression", "");

    // Assume expression is a single keyword "skip" for this example
    if (have("keyword", "skip")) {
        expressionNode->addChild(new ParseTree("keyword", "skip"));
        next();
    } else {
        throw ParseException();
    }

    return expressionNode;
}

/**
 * Generates a parse tree for an expression term
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileTerm() {
    return NULL;
}

/**
 * Generates a parse tree for an expression list
 * @return a ParseTree
 */
ParseTree* CompilerParser::compileExpressionList() {
    return NULL;
}

/**
 * Advance to the next token
 */
void CompilerParser::next() {
    if (currentToken != tokens.end()) {
        ++currentToken;
    }
}

// Return the current token
Token* CompilerParser::current() {
    if (currentToken != tokens.end()) {
        return *currentToken;
    }
    return nullptr;
}

// Check if the current token matches the expected type and value
bool CompilerParser::have(std::string expectedType, std::string expectedValue) {
    Token* token = current();
    return token != nullptr && token->getType() == expectedType && token->getValue() == expectedValue;
}

// Check if the current token matches the expected type and value
// If so, advance to the next token, returning the current token, otherwise throw a ParseException
Token* CompilerParser::mustBe(std::string expectedType, std::string expectedValue) {
    Token* token = current();
    if (token == nullptr || token->getType() != expectedType || (expectedValue != "" && token->getValue() != expectedValue)) {
        throw ParseException();
    }
    next();
    return token;
}
/**
 * Definition of a ParseException
 * You can use this ParseException with `throw ParseException();`
 */
const char* ParseException::what() {
    return "An Exception occurred while parsing!";
}
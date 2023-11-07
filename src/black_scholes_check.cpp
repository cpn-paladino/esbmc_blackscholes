#include <ql/quantlib.hpp>
#include <memory>
#include <cassert>

using namespace QuantLib;

int main() {
    // Parâmetros de mercado
    Real S = 100.0; // Preço atual do ativo subjacente
    Real K = 100.0; // Preço de exercício
    Spread dividendYield = 0.02;
    Rate riskFreeRate = 0.05;
    Volatility volatility = 0.20;
    Date maturity(15, May, 2024);
    DayCounter dayCounter = Actual365Fixed();
    Calendar calendar = TARGET();

    // Calcula a data de exercício como um objeto QuantLib
    EuropeanExercise europeanExercise(maturity);

    // Configurando o processo de Black-Scholes-Merton
    // Cria os 'Handle' para as estruturas de termo necessárias
    Handle<Quote> underlyingH(boost::make_shared<SimpleQuote>(S));
    Handle<YieldTermStructure> dividendTS(boost::make_shared<FlatForward>(0, calendar, dividendYield, dayCounter));
    Handle<YieldTermStructure> riskFreeTS(boost::make_shared<FlatForward>(0, calendar, riskFreeRate, dayCounter));
    Handle<BlackVolTermStructure> volatilityTS(boost::make_shared<BlackConstantVol>(0, calendar, volatility, dayCounter));

    BlackScholesMertonProcess bsmProcess(
        underlyingH,
        dividendTS,
        riskFreeTS,
        volatilityTS
    );

    // Opção de chamada europeia
    Option::Type optionType = Option::Call;

    // Construindo o payoff
    boost::shared_ptr<StrikedTypePayoff> payoff(new PlainVanillaPayoff(optionType, K));

    // Construindo o exercício europeu
    boost::shared_ptr<Exercise> exercise(new EuropeanExercise(maturity));

    // Criando a opção europeia
    VanillaOption europeanOption(payoff, exercise);

    // Parâmetros para o cálculo do preço da opção
    Time timeToMaturity = dayCounter.yearFraction(Settings::instance().evaluationDate(), maturity);    
    Real growthFactor = std::exp(dividendYield * timeToMaturity);    
    DiscountFactor discountFactor = std::exp(-riskFreeRate * timeToMaturity);    
    Volatility volStdDev = volatility * std::sqrt(timeToMaturity);


    // Assert para garantir que o fator de crescimento é positivo e razoável
    assert(growthFactor > 1.0 && "O fator de crescimento deve ser maior que 1 para dividendos positivos.");
    
    // Assert para garantir que o fator de desconto é entre 0 e 1
    assert(discountFactor > 0.0 && discountFactor < 1.0 && "O fator de desconto deve estar entre 0 e 1.");

    // Assert para garantir que a volatilidade padronizada é positiva
    assert(volStdDev > 0.0 && "A volatilidade padronizada deve ser positiva.");

    // Criando o calculador de Black-Scholes
    BlackScholesCalculator bsc(payoff, S, growthFactor, volStdDev, discountFactor);

    // Avaliando o preço da opção
    Real optionPrice = bsc.value();

    // Assert para garantir que o preço da opção é positivo
    assert(optionPrice > 0.0 && "O preço da opção deve ser positivo.");

    // Assert para verificar se o modelo Black-Scholes calcula um preço dentro de um intervalo esperado para dados de entrada conhecidos
    assert(optionPrice > S * discountFactor - K * growthFactor && "O preço da opção está abaixo do limite inferior teórico.");

    // Saída do preço da opção
    std::cout << "The theoretical price of the option is: " << optionPrice << std::endl;

    return 0;
}
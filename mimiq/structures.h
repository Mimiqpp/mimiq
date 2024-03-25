#ifndef _structures
#define _structures
#define root2 std::sqrt(2)
#define PI acos(0.0) * 2
struct GlobalInitializer {
    GlobalInitializer() {
        std::cout << "mimiQ++ \n";
        std::srand(static_cast < unsigned int > (std::time(nullptr)));
    }
}
initializer;
struct Coeff {
    double real;
    double complex;
    Coeff(): real(0), complex(0) {}
    Coeff(double r, double c): real(r), complex(c) {}
    Coeff operator * (const Coeff & other) const {
        Coeff result;
        result.real = real * other.real - complex * other.complex;
        result.complex = real * other.complex + complex * other.real;
        return result;
    }
    Coeff operator + (const Coeff & other) const {
        Coeff result;
        result.real = real + other.real;
        result.complex = complex + other.complex;
        return result;
    }
    Coeff operator - (const Coeff & other) const {
        Coeff result;
        result.real = real - other.real;
        result.complex = complex - other.complex;
        return result;
    }
    Coeff operator / (const Coeff & other) const {
        Coeff result;
        result.real = ((real * other.real) + (complex * other.complex)) / ((other.real * other.real) + (other.complex * other.complex));
        result.complex = ((other.complex * real) - (real * other.complex)) / ((other.real * other.real) + (other.complex * other.complex));
        return result;
    }
    double amplitude() const { //amp squared actually
        return (real * real) + (complex * complex);
    }
};
struct state_vector {
    int n_qbits;
    std::vector < Coeff > coeffs;
    state_vector(): n_qbits(0) {}
    state_vector(int n) {
        n_qbits = n;
        coeffs.resize(1 << n, { 0, 0 });
    }
    state_vector& operator=(const state_vector& other) {
        if (this != &other) { // Check for self-assignment
            n_qbits = other.n_qbits;
            coeffs = other.coeffs; 
        }
        return *this;
    }
    state_vector(const struct state_vector & sv) {
        n_qbits = sv.n_qbits;
        coeffs = sv.coeffs;
    }
    struct state_vector operator * (const struct state_vector & other) // returns a tensor product of 2 state vectors 
    {
        struct state_vector sv3; //(*this).n_qbits == n_qbits 
        sv3.n_qbits = n_qbits + other.n_qbits;
        int cnd1 = 1 << n_qbits, cnd2 = 1 << other.n_qbits;
        for(int i = 0; i < cnd1; i++)
            for(int j = 0; j < cnd2; j++) sv3.coeffs.push_back(coeffs[i] * other.coeffs[j]);
        return sv3;
    }
    void print() {
        std::cout << std::endl << "number of qbits = " << n_qbits << std::endl;
        int cnd = 1 << n_qbits;
        for(int i = 0; i < cnd; i++) {
            std::string binaryString;
            for(int j = n_qbits - 1; j >= 0; --j) {
                int bit = (i >> j) & 1;
                binaryString += (bit == 0) ? '0' : '1';
            }
            std::cout << binaryString << " =>" << coeffs[i].amplitude() * 100 << "% ( " << coeffs[i].real << " |0> + " << coeffs[i].complex << " i |1> )\n";
        }
        std::cout << std::endl;
    }
    void printprobs()
    {
        int cnd = 1 << n_qbits ;
        for(int bit = 0; bit <  n_qbits ; bit++)
        {
            auto tmask = 1 << (n_qbits - 1 - bit);
            double prob = 0.0;
            for(int i = 0; i < cnd; i++)
                if(i & tmask) prob += coeffs[i].amplitude();
            std::cout << "for qbit " << bit << ": " <<"Prob of |1> : " << prob << ",  ";
            std::cout << "Prob of |0> : " << 1 - prob << std::endl;
        }
        std::cout<<std::endl;
    }
};
struct result {
    int n_cbits;
    std::map < int, int > m; // TODO rename, restructure 
    struct state_vector state;
    void print_state() {
        state.print();
    }
    void get_counts() {
        std::cout << "classical register readings for the simulation: " << std::endl;
        for(auto i = m.begin(); i != m.end(); i++) {
            std::string binaryString;
            for(int j = n_cbits - 1; j >= 0; --j) {
                int bit = (i -> first >> j) & 1;
                binaryString += (bit == 0) ? '0' : '1';
            }
            std::cout << binaryString << ": " << i -> second << std::endl;
        }
        std::cout << std::endl;
    }
    void get_probs()
    {
        state.printprobs();
    }
};
std::vector < std::vector < Coeff >> ma1, ma2, ma3;
#endif

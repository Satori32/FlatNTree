#include <iostream>
#include <vector>
#include <cstdint>
#include <tuple>
#include <algorithm>

template<class T>
class FlatNTree {
public:
	FlatNTree() {
		Root = Pointer = Count;
		Node N;
		N.N = Count++;
		Nodes.push_back(N);
	}
	FlatNTree(const T& In) {
		Root = Pointer = Count;
		Node N;
		N.N = Count++;
		N.Value = In;
		Nodes.pop_back(N);
	}

	bool MoveParent() {
		auto X = Search(Pointer);
		if (!std::get<0>(X)) { return false; }
		auto Y = Search(std::get<1>(X).Parent);
		if (!std::get<0>(Y)) { return false; }
		Pointer = std::get<1>(X).Parent;
		return true;
	}
	bool MoveChild(std::size_t P) {
		auto X = Search(Pointer);
		if (!std::get<0>(X)) { return false; }

		Pointer = std::get<1>(X).Child[P];

		return true;
	}
	std::size_t GetChildCount() {
		auto X = Search(Pointer);
		if (!std::get<0>(X)) { return 0; }

		return std::get<1>(X).Child.size();
	}
	bool PushChild(const T& In) {

		auto X = Search(Pointer);
		if (!std::get<0>(X)) { return false; }

		Node N;
		N.N = Count++;
		std::get<1>(X).Child.push_back(N.N);

		N.Parent = Pointer;
		N.Value = In;
		Nodes.push_back(N);

		return true;
	}
	bool PopChild(std::size_t P) {
		auto X = Search(Pointer);
		if (!std::get<0>(X)) { return false; }
		if (std::get<1>(X).Child.size() <= P) { return false; }
		auto Y = Search(std::get<1>(X).Child[P]);
		if (!std::get<0>(Y)) { return false; }
		for (auto& o : std::get<1>(Y).Child) {
			auto Z = Search(o);
			if (!std::get<0>(Z)) { return false; }
			std::get<1>(Z).Parent = Pointer;
		}
		std::get<1>(X).Child.insert(std::get<1>(X).Child.end(), std::get<1>(Y).Child.begin(), std::get<1>(Y).Child.end());
		std::get<1>(X).Child.erase(std::get<1>(X).Child.begin() + P);
		return true;
	}

	const std::tuple<bool, T&> GetValue() {
		auto X = Search(Pointer);
		if (!std::get<0>(X)) { return { false ,std::get<1>(X).Value }; };


		return { true,std::get<1>(X).Value };
	}
	bool SetValue(const T& In) {
		auto X = Search(Pointer);
		if (!std::get<0>(X)) { return false; }

		std::get<1>(X).Value = In;
		return true;
	}
	bool MoveRoot() {
		Pointer == Root;
		return true;
	}
	std::size_t Size() {
		return Nodes.size();
	}
protected:

	struct Node
	{
	public:
		T Value;
		std::uintmax_t N = 0;
		std::uintmax_t Parent = 0;
		std::vector<std::uintmax_t> Child;
	};

	std::tuple<bool, typename FlatNTree<T>::Node&> Search(std::uintmax_t N) {
		std::size_t L = Nodes.size();
		std::size_t Min = 0;
		std::size_t Max = L;
		std::size_t Mid = L / 2;
		std::sort(Nodes.begin(), Nodes.end(), [](Node& A, Node& B) {return A.N < B.N; });

		while (Min < Max) {
		
			Mid = (Max - Min) / 2;
			if (Nodes[Mid].N > N) {
				Min = Mid;
			}
			else if (Nodes[Mid].N < N) {
				Max = Mid;
			}
			else {
				break;
			}
		}

		return { N == Nodes[Mid].N,Nodes[Mid] };
	}

	std::vector<Node> Nodes;
	std::uintmax_t Root = 0;
	std::uintmax_t Pointer = 0;
	std::uintmax_t Count = 0;
};

int main() {
	FlatNTree<int> T;

	T.SetValue(10);

	std::cout << std::get<1>(T.GetValue()) << std::endl;

	T.PushChild(8);
	T.MoveChild(0);
	std::cout << std::get<1>(T.GetValue()) << std::endl;
	T.MoveParent();
	std::cout << T.GetChildCount() << std::endl;
	T.PopChild(0);

	std::cout << T.GetChildCount() << std::endl;

	return 0;
}
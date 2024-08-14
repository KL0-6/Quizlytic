import React, { useEffect, useState } from "react";
import Modal from "../../components/Modal/Modal";
import CreateFlashcard from "../CreateFlashcard/CreateFlashcard";
import { useUser, SignedIn, SignedOut, SignIn } from '@clerk/clerk-react';

interface Props 
{
    onSelectSet: (title: string, data: string) => void;
}

const FlashcardSetSelection: React.FC<Props> = ({ onSelectSet }) => 
{
    const [searchTerm, setSearchTerm] = useState("");
    const [isModalOpen, setIsModalOpen] = useState(false);
    const { user } = useUser();

    const [flashcardSets, setFlashcardSets] = useState<any[]>([]);

    useEffect(() => 
    {
        if (user) 
        {
            fetch(`http://localhost:8848/getflashcards?userId=${user.id}`).then((response) => 
            {
                if (!response.ok) 
                    throw new Error('Failed to fetch flashcards');

                return response.json();
            })
            .then((data) => 
            {
                const fetchedSets = data.map((flashcard: any) => 
                {
                    console.log(flashcard);

                    return {
                        hash: flashcard.hash,
                        name: flashcard.title,
                        description: flashcard.description,
                        data: flashcard.data
                    };
                });

                setFlashcardSets(fetchedSets);
            })
            .catch((error) => 
            {
                console.error("Error fetching flashcards:", error);
            });
        }
    }, [user]);

    const filteredSets = flashcardSets.filter((set) =>
        set.name.toLowerCase().includes(searchTerm.toLowerCase())
    );

    const openModal = () => setIsModalOpen(true);
    const closeModal = () => setIsModalOpen(false);

    const onModalSubmit = (title: string, description: string, hash: string, data: string) => 
    {
        const newSet = 
        {
            hash: hash, 
            name: title,
            description: description,
            data: data
        };

        setFlashcardSets([...flashcardSets, newSet]);
        setIsModalOpen(false);
    }

    return (
        <div className="min-h-screen bg-gradient-to-b from-black via-gray-900 to-black text-white flex flex-col items-center pt-20 md:pt-24 p-4 md:p-6">
            <SignedOut>
                <div className="flex flex-col items-center justify-center text-center">
                    <h1 className="text-xl md:text-2xl font-bold mb-4 md:mb-6">Please sign in to access the flashcard sets</h1>
                    <SignIn />
                </div>
            </SignedOut>

            <SignedIn>
                <h1 className="text-lg md:text-xl font-bold mb-4">Welcome, {user?.firstName}!</h1>

                <h2 className="text-xl md:text-2xl font-bold mb-4 md:mb-6 text-center">Select a Flashcard Set</h2>

                <div className="flex flex-col w-full max-w-3xl mb-12 md:mb-16 space-y-4 md:space-y-0">
                    <div className="flex flex-row items-center mb-8">
                        <input type="text" placeholder="Search flashcard sets..." value={searchTerm} onChange={(e) => setSearchTerm(e.target.value)} className="w-full px-4 py-2 bg-gray-800 text-white rounded-lg focus:outline-none focus:ring-2 focus:ring-green-500 h-12"/>
                        <button className="bg-green-600 text-white rounded-lg shadow-md hover:bg-green-500 ml-4 h-12 flex items-center justify-center px-4" onClick={openModal}>Create New Set</button>
                    </div>

                    <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 gap-6 w-full">
                        {filteredSets.length > 0 ? (
                            filteredSets.map((set) => (
                                <div key={set.hash} className="p-6 bg-gray-800 rounded-lg cursor-pointer hover:bg-gray-700 flex flex-col justify-between h-40" onClick={() => onSelectSet(set.name, set.data)}>
                                    <h3 className="text-lg font-semibold">{set.name}</h3>
                                    <p className="text-gray-400">{set.description}</p>
                                </div>
                            ))
                        ) : (
                            <p className="text-gray-400 col-span-full text-center">No flashcard sets found.</p>
                        )}
                    </div>
                </div>

                <Modal isOpen={isModalOpen}>
                    <CreateFlashcard onClose={closeModal} onSubmit={onModalSubmit} />
                </Modal>
            </SignedIn>
        </div>
    );
};

export default FlashcardSetSelection;

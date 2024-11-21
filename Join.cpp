#include "Join.hpp"

#include <vector>

using namespace std;

/*
 * Input: Disk, Memory, Disk page ids for left relation, Disk page ids for right relation
 * Output: Vector of Buckets of size (MEM_SIZE_IN_PAGE - 1) after partition
 */
vector<Bucket> partition(Disk* disk, Mem* mem, pair<uint, uint> left_rel,
                         pair<uint, uint> right_rel) {
	
	vector<Bucket> partitions(MEM_SIZE_IN_PAGE - 1, Bucket(disk)); 

	for(uint i = left_rel.first; i < left_rel.second; ++i){
		mem->loadFromDisk(disk, i, MEM_SIZE_IN_PAGE - 1);
		Page* curPage = mem->mem_page(MEM_SIZE_IN_PAGE - 1);

		for(uint j = 0; j < curPage->size(); ++j){
			uint h = curPage->get_record(j).partition_hash();
			uint h1 = h%MEM_SIZE_IN_PAGE - 1;
			Page* memPage = mem->mem_page(h1);

			memPage->loadRecord(curPage->get_record(j));

			if(memPage->full()){
				partitions[h1].add_left_rel_page(mem->flushToDisk(disk, h1));
			}
		}
		curPage->reset();

	}
	for(uint i = 0; i < partitions.size(); ++i){
		if(!mem->mem_page(i)->empty()){
			partitions[i].add_left_rel_page(mem->flushToDisk(disk, i));
		}
	}

	mem->reset();

	for(uint i = right_rel.first; i < right_rel.second; ++i){
		mem->loadFromDisk(disk, i, MEM_SIZE_IN_PAGE - 1);
		Page* curPage = mem->mem_page(MEM_SIZE_IN_PAGE - 1);

		for(uint j = 0; j < curPage->size(); ++j){
			uint h = curPage->get_record(j).partition_hash();
			uint h1 = h%MEM_SIZE_IN_PAGE - 1;
			Page* memPage = mem->mem_page(h1);

			memPage->loadRecord(curPage->get_record(j));

			if(memPage->full()){
				partitions[h1].add_right_rel_page(mem->flushToDisk(disk, h1));
			}
		}
		curPage->reset();

	}
	for(uint i = 0; i < partitions.size(); ++i){
		if(!mem->mem_page(i)->empty()){
			partitions[i].add_right_rel_page(mem->flushToDisk(disk, i));
		}
	}

	mem->reset();

	return partitions;
}

/*
 * Input: Disk, Memory, Vector of Buckets after partition
 * Output: Vector of disk page ids for join result
 */
vector<uint> probe(Disk* disk, Mem* mem, vector<Bucket>& partitions) {

	vector<uint> larger;
	vector<uint> smaller;

	vector<uint> disk_pages;

	for(uint i = 0; i < MEM_SIZE_IN_PAGE - 2; ++i){
		if(partitions[i].num_left_rel_record >= partitions[i].num_right_rel_record){
			larger = partitions[i].get_left_rel();
			smaller = partitions[i].get_right_rel();
		}
		else{
			larger = partitions[i].get_right_rel();
			smaller = partitions[i].get_left_rel();
		}

		for(uint j = 0; j < larger.size(); ++j){
			mem->loadFromDisk(disk, larger[j], MEM_SIZE_IN_PAGE - 2);
			Page* curPage = mem->mem_page(MEM_SIZE_IN_PAGE - 2);

			for(uint k = 0; k < curPage->size(); ++k){
				uint h = curPage->get_record(k).probe_hash();
				uint h2 = h%MEM_SIZE_IN_PAGE - 2;
				Page* memPage = mem->mem_page(h2);

				memPage->loadRecord(curPage->get_record(k));
			}
			curPage->reset();
		}

		for(uint j = 0; j < smaller.size(); ++j){
			mem->loadFromDisk(disk, smaller[j], MEM_SIZE_IN_PAGE - 2);
			Page* curPage = mem->mem_page(MEM_SIZE_IN_PAGE - 2);

			for(uint k = 0; k < curPage->size(); ++k){
				uint h = curPage->get_record(k).probe_hash();
				uint h2 = h%MEM_SIZE_IN_PAGE - 2;
				Page* memPage = mem->mem_page(h2);

				for(uint l = 0; l < memPage->size(); ++l){
					if(memPage->get_record(l) == curPage->get_record(k)){
						Page* outPage = mem->mem_page(MEM_SIZE_IN_PAGE - 1);
						outPage->loadPair(memPage->get_record(l), curPage->get_record(k));

						if(outPage->full()){
							disk_pages.push_back(mem->flushToDisk(disk, MEM_SIZE_IN_PAGE - 1));
						}
					}
				}
			}
			curPage->reset();
		}
	}

	if(!mem->mem_page(MEM_SIZE_IN_PAGE - 1)->empty()){
		disk_pages.push_back(mem->flushToDisk(disk, MEM_SIZE_IN_PAGE - 1));
	}
	
	return disk_pages;
}

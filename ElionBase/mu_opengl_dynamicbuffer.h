#ifndef __MU_OPENGL_DYNAMICBUFFER_H__
#define __MU_OPENGL_DYNAMICBUFFER_H__

#pragma once

namespace EOpenGLUtils
{
	struct EMappedMemoryRange
	{
		mu_size offset;
		mu_size size;
	};

	EINLINE void ParseMappedRangesFromMap(std::map<mu_uint32, mu_uint32> &Input, std::vector<EMappedMemoryRange> &Output, const mu_size maxSize, const mu_size dataSize)
	{
		for (auto it = Input.begin(); it != Input.end(); ++it)
		{
			mu_boolean isParsed = false;

			mu_size leftOffset = it->first * dataSize;
			mu_size leftSize = it->second * dataSize;
			const mu_size leftEndOffset = leftOffset + leftSize;

			auto mappedit = Output.begin();
			for (; mappedit != Output.end(); ++mappedit)
			{
				EMappedMemoryRange &rightRange = *mappedit;
				const mu_size rightEndOffset = rightRange.offset + rightRange.size;

				if (leftOffset <= rightRange.offset)
				{
					if (leftEndOffset >= rightRange.offset)
					{
						rightRange.offset = leftOffset;
						if (leftEndOffset >= rightEndOffset)
						{
							rightRange.size = leftSize;
						}
						else
						{
							rightRange.size = leftSize + (rightEndOffset - leftEndOffset);
						}

						isParsed = true;
					}
					break;
				}
				else if (leftOffset <= rightEndOffset)
				{
					if (leftEndOffset >= rightEndOffset)
					{
						rightRange.size += (leftEndOffset - rightEndOffset);
					}

					isParsed = true;
					break;
				}
			}

			if (isParsed == false)
			{
				EMappedMemoryRange tmpMappedRange = {};
				tmpMappedRange.offset = leftOffset;
				tmpMappedRange.size = leftSize;

				Output.insert(mappedit, tmpMappedRange);
			}
		}
	}

	EINLINE void ParseMappedRangesFromVector(std::vector<EMappedMemoryRange> &Input, std::vector<EMappedMemoryRange> &Output)
	{
		for (auto it = Input.begin(); it != Input.end(); ++it)
		{
			mu_boolean isParsed = false;

			const EMappedMemoryRange &leftRange = *it;
			const mu_size leftEndOffset = leftRange.offset + leftRange.size;

			auto mappedit = Output.begin();
			for (; mappedit != Output.end(); ++mappedit)
			{
				EMappedMemoryRange &rightRange = *mappedit;
				const mu_size rightEndOffset = rightRange.offset + rightRange.size;

				if (leftRange.offset <= rightRange.offset)
				{
					if (leftEndOffset >= rightRange.offset)
					{
						rightRange.offset = leftRange.offset;
						if (leftEndOffset >= rightEndOffset)
						{
							rightRange.size = leftRange.size;
						}
						else
						{
							rightRange.size = leftRange.size + (rightEndOffset - leftEndOffset);
						}

						isParsed = true;
					}
				}
				else if (leftRange.offset <= rightEndOffset)
				{
					if (leftEndOffset >= rightEndOffset)
					{
						rightRange.size += (leftEndOffset - rightEndOffset);
					}

					isParsed = true;
				}
			}

			if (isParsed == false)
			{
				Output.insert(mappedit, leftRange);
			}
		}
	}

	EINLINE void ParseMappedRanges(std::map<mu_uint32, mu_uint32> &Input, std::vector<EMappedMemoryRange> &Output, const mu_size maxSize, const mu_size dataSize)
	{
		std::vector<EMappedMemoryRange> tmpOutput;
		ParseMappedRangesFromMap(Input, tmpOutput, maxSize, dataSize);
		ParseMappedRangesFromVector(tmpOutput, Output);
	}

	EINLINE void ParseBufferCopy(std::vector<EMappedMemoryRange> &Input, std::vector<EMappedMemoryRange> &Output)
	{
		std::vector<EMappedMemoryRange> tmp;
		tmp.insert(tmp.end(), Input.begin(), Input.end());
		ParseMappedRangesFromVector(Output, tmp);
		Output.clear();
		ParseMappedRangesFromVector(tmp, Output);
	}

	struct EDynamicBufferCopies
	{
		std::vector<EMappedMemoryRange> Data;
	};

	template<class T, GLenum TargetType, mu_uint32 InitialSize, mu_uint32 GrowSize>
	class EDynamicBuffer
	{
	public:
		EDynamicBuffer()
		{
			_StagingIndex = 0;
			_StagingCount = 0;
			_BufferMemory = nullptr;
		}

		~EDynamicBuffer()
		{
			if (_Buffers.empty() == false)
			{
				// Dynamic buffer not destroyed!
				DEBUG_BREAK();
			}
		}

		EINLINE const mu_boolean ResizeBuffer(const mu_uint32 count)
		{
			T *newBufferMemory = reinterpret_cast<T*>(mu_realloc(_BufferMemory, count * sizeof(T)));
			if(newBufferMemory == nullptr)
			{
				return false;
			}

			_StagingCount = count;
			std::fill(_BuffersResize.begin(), _BuffersResize.end(), true);
			_StagingUpdateMap.clear();
			_BufferMemory = newBufferMemory;

			return true;
		}
		EINLINE const mu_boolean Initialize(const mu_uint32 SwapchainCount)
		{
			_BuffersResize.resize(SwapchainCount, false);
			_Buffers.resize(SwapchainCount, 0);
			_BufferCopies.resize(SwapchainCount);
			EOpenGLHelpers::GenerateBuffers(SwapchainCount, _Buffers.data());

			if (_Buffers[0] == 0)
			{
				return false;
			}

			return ResizeBuffer(InitialSize);
		}
		EINLINE void Destroy(const mu_boolean gpuOnly = false)
		{
			if (_Buffers.empty() == false &&
				_Buffers[0] != 0)
			{
				EOpenGLHelpers::DeleteBuffers(_Buffers.size(), _Buffers.data());
			}

			if (gpuOnly == false)
			{
				if (_BufferMemory != nullptr)
				{
					mu_free(_BufferMemory);
					_BufferMemory = nullptr;
				}

				_Buffers.clear();

				_StagingIndex = 0;
				_StagingCount = 0;
				_StagingUpdateMap.clear();
			}
			else
			{
				RequestUpdateData(0, _StagingIndex);
			}
		}

		EINLINE void UpdateBuffer()
		{
			const mu_boolean disabledTransfers = EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false;
			const mu_uint32 currentIndex = EOpenGL::CurrentCommandBufferIndex;

			if (disabledTransfers == false && _BuffersResize[currentIndex] == true)
			{
				glBindBuffer(TargetType, _Buffers[currentIndex]);
				glBufferData(TargetType, _StagingCount * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
				glBufferSubData(TargetType, 0, _StagingIndex * sizeof(T), _BufferMemory);
				glBindBuffer(TargetType, 0);
				_BuffersResize[currentIndex] = false;
			}

			// Update Staging Memory
			if (_StagingUpdateMap.empty() == false)
			{
				const mu_size maxStagingSize = _StagingCount * sizeof(T);

				std::vector<EMappedMemoryRange> mappedRanges;
				ParseMappedRanges(_StagingUpdateMap, mappedRanges, maxStagingSize, sizeof(T));

				for (auto it = _BufferCopies.begin(); it != _BufferCopies.end(); ++it)
				{
					EDynamicBufferCopies &data = *it;
					ParseBufferCopy(mappedRanges, data.Data);
				}

				_StagingUpdateMap.clear();
			}

			EDynamicBufferCopies &copies = _BufferCopies[currentIndex];
			if (disabledTransfers == false && copies.Data.size() > 0)
			{
				glBindBuffer(TargetType, _Buffers[currentIndex]);
				for (auto it = copies.Data.begin(); it != copies.Data.end(); ++it)
				{
					EMappedMemoryRange &tmp = *it;
					glBufferSubData(TargetType, tmp.offset, tmp.size, reinterpret_cast<mu_uint8*>(_BufferMemory) + tmp.offset);
				}
				glBindBuffer(TargetType, 0);
				copies.Data.clear();
			}
		}

		EINLINE const mu_uint32 AllocateDataIndex(const mu_uint32 count)
		{
			if (_StagingIndex + count > _StagingCount)
			{
				const mu_uint32 diffSize = (_StagingIndex + count) - _StagingCount;
				const mu_uint32 newSize = _StagingCount + diffSize + (GrowSize - diffSize % GrowSize) % GrowSize;
				if (ResizeBuffer(newSize) == false)
				{
					return EInvalidUInt32;
				}
			}

			const mu_uint32 index = _StagingIndex;
			_StagingIndex += count;

			return index;
		}

		EINLINE T *RetrieveDataBuffer(const mu_uint32 index)
		{
			return &_BufferMemory[index];
		}

		EINLINE void RequestUpdateData(const mu_uint32 index, const mu_uint32 count)
		{
			auto it = _StagingUpdateMap.find(index);
			if (it != _StagingUpdateMap.end())
			{
				if (it->second < count)
				{
					it->second = count;
				}
			}
			else
			{
				_StagingUpdateMap.insert(std::make_pair(index, count));
			}

			UpdateBuffer();
		}

		EINLINE GLuint RetrieveBuffer(const mu_uint32 index)
		{
			return _Buffers[index];
		}

	private:
		mu_uint32 _StagingIndex;
		mu_uint32 _StagingCount;
		std::map<mu_uint32, mu_uint32> _StagingUpdateMap;

		T *_BufferMemory;
		std::vector<mu_boolean> _BuffersResize;
		std::vector<GLuint> _Buffers;
		std::vector<EDynamicBufferCopies> _BufferCopies;
	};

	template<class T, GLenum TargetType, mu_uint32 InitialSize, mu_uint32 GrowSize>
	class EDynamicBufferExtended
	{
	public:
		EDynamicBufferExtended()
		{
			_StagingIndex = 0;
			_StagingCount = 0;
			_BufferMemory = nullptr;

			_MemoryFragmented = false;
		}
		~EDynamicBufferExtended()
		{
			if (_Buffers.empty() == false)
			{
				// Dynamic buffer not destroyed!
				DEBUG_BREAK();
			}
		}

		EINLINE const mu_boolean ResizeBuffer(const mu_uint32 count)
		{
			T *newBufferMemory = reinterpret_cast<T*>(mu_realloc(_BufferMemory, count * sizeof(T)));
			if (newBufferMemory == nullptr)
			{
				return false;
			}

			_StagingCount = count;
			std::fill(_BuffersResize.begin(), _BuffersResize.end(), true);
			_StagingUpdateMap.clear();
			_BufferMemory = newBufferMemory;

			return true;
		}
		EINLINE const mu_boolean Initialize(const mu_uint32 SwapchainCount)
		{
			_BuffersResize.resize(SwapchainCount, false);
			_Buffers.resize(SwapchainCount, 0);
			_BufferCopies.resize(SwapchainCount);
			EOpenGLHelpers::GenerateBuffers(SwapchainCount, _Buffers.data());

			if (_Buffers[0] == 0)
			{
				return false;
			}

			return ResizeBuffer(InitialSize);
		}
		EINLINE void Destroy(const mu_boolean gpuOnly = false)
		{
			if (_Buffers.empty() == false &&
				_Buffers[0] != 0)
			{
				EOpenGLHelpers::DeleteBuffers(_Buffers.size(), _Buffers.data());
			}

			if (gpuOnly == false)
			{
				if (_BufferMemory != nullptr)
				{
					mu_free(_BufferMemory);
					_BufferMemory = nullptr;
				}

				_Buffers.clear();

				_StagingIndex = 0;
				_StagingCount = 0;
				_StagingUpdateMap.clear();

				_MemoryFragmented = false;
				_MemoryFreeIndexes.clear();
				_MemoryIndexesMap.clear();
			}
			else
			{
				RequestUpdateDataRaw(0, _StagingIndex);
			}
		}

		EINLINE void DefragmentBuffer()
		{
			if (_MemoryFragmented == true)
			{
				if (_MemoryFreeIndexes.empty() == false)
				{
					for (auto it = _MemoryFreeIndexes.begin(); it != _MemoryFreeIndexes.end(); ++it)
					{
						const mu_uint32 index = *it;
						EDynamicIndex &dynamicIndex = _MemoryIndexesMap[index];
						dynamicIndex.Count = 0;
					}
				}

				std::vector<EDynamicIndex> memoryZones;
				for (auto it = _MemoryIndexesMap.begin(); it != _MemoryIndexesMap.end(); ++it)
				{
					const EDynamicIndex &leftRange = *it;

					if (leftRange.Count == 0) continue;

					const mu_uint32 leftEndOffset = leftRange.Index + leftRange.Count;

					mu_boolean isParsed = false;
					auto zoneiter = memoryZones.begin();
					for (; zoneiter != memoryZones.end(); ++zoneiter)
					{
						EDynamicIndex &rightRange = *zoneiter;

						const mu_uint32 rightEndOffset = rightRange.Index + rightRange.Count;

						if (leftRange.Index <= rightRange.Index)
						{
							if (leftEndOffset >= rightRange.Index)
							{
								rightRange.Index = leftRange.Index;
								if (leftEndOffset >= rightEndOffset)
								{
									rightRange.Count = leftRange.Count;
								}
								else
								{
									rightRange.Count = leftRange.Count + (rightEndOffset - leftEndOffset);
								}

								isParsed = true;
							}
							break;
						}
						else if (leftRange.Index <= rightEndOffset)
						{
							if (leftEndOffset >= rightEndOffset)
							{
								rightRange.Count += (leftEndOffset - rightEndOffset);
							}

							isParsed = true;
							break;
						}
					}

					if (isParsed == false)
					{
						memoryZones.insert(zoneiter, leftRange);
					}
				}

				std::vector<EDynamicIndex> fragmentedZones;
				for (auto it = memoryZones.begin(); it != memoryZones.end(); ++it)
				{
					mu_boolean isParsed = false;

					const EDynamicIndex &leftRange = *it;
					const mu_uint32 leftEndOffset = leftRange.Index + leftRange.Count;

					auto mappedit = fragmentedZones.begin();
					for (; mappedit != fragmentedZones.end(); ++mappedit)
					{
						EDynamicIndex &rightRange = *mappedit;
						const mu_uint32 rightEndOffset = rightRange.Index + rightRange.Count;

						if (leftRange.Index <= rightRange.Index)
						{
							if (leftEndOffset >= rightRange.Index)
							{
								rightRange.Index = leftRange.Index;
								if (leftEndOffset >= rightEndOffset)
								{
									rightRange.Count = leftRange.Count;
								}
								else
								{
									rightRange.Count = leftRange.Count + (rightEndOffset - leftEndOffset);
								}

								isParsed = true;
							}
							break;
						}
						else if (leftRange.Index <= rightEndOffset)
						{
							if (leftEndOffset >= rightEndOffset)
							{
								rightRange.Count += (leftEndOffset - rightEndOffset);
							}

							isParsed = true;
							break;
						}
					}

					if (isParsed == false)
					{
						fragmentedZones.insert(mappedit, leftRange);
					}
				}

				const mu_uint32 fragmentedCount = fragmentedZones.size();
				if (fragmentedCount > 1)
				{
					std::vector<mu_uint32> decreaseOffsets(fragmentedZones.size(), 0);

					// Parse Decrease Offsets
					{
						mu_uint32 decreasedOffset = 0;
						auto decreaseiter = decreaseOffsets.begin() + 1;
						for (auto aiter = fragmentedZones.begin(),
							biter = aiter + 1;
							biter != fragmentedZones.end();
							++aiter, ++biter, ++decreaseiter)
						{
							EDynamicIndex &zoneA = *aiter;
							EDynamicIndex &zoneB = *biter;

							const mu_uint32 leftEndOffset = zoneA.Index + zoneA.Count;
							decreasedOffset += (zoneB.Index - leftEndOffset);
							*decreaseiter = decreasedOffset;
						}
					}

					// Decrease Indexes
					{
						const EDynamicIndex &firstZone = fragmentedZones[0];
						const mu_uint32 firstZoneIndex = firstZone.Index;
						const mu_uint32 firstZoneEndOffset = firstZone.Index + firstZone.Count;

						for (auto it = _MemoryIndexesMap.begin(); it != _MemoryIndexesMap.end(); ++it)
						{
							EDynamicIndex &dynamicIndex = *it;
							if (dynamicIndex.Count == 0) continue;

							const mu_uint32 dynamicEndOffset = dynamicIndex.Index + dynamicIndex.Count;

							if (dynamicIndex.Index >= firstZoneIndex &&
								dynamicEndOffset <= firstZoneEndOffset)
							{
								continue;
							}

							auto zoneiter = fragmentedZones.begin() + 1;
							auto decreaseiter = decreaseOffsets.begin() + 1;
							for (; zoneiter != fragmentedZones.end(); ++zoneiter, ++decreaseiter)
							{
								const EDynamicIndex &zoneIndex = *zoneiter;
								const mu_uint32 zoneEndOffset = zoneIndex.Index + zoneIndex.Count;

								if (dynamicIndex.Index >= zoneIndex.Index &&
									dynamicEndOffset <= zoneEndOffset)
								{
									dynamicIndex.Index -= *decreaseiter;
									break;
								}
							}
						}
					}

					// Defragment Memory Zones
					{
						auto decreaseiter = decreaseOffsets.begin() + 1;
						for (auto zoneiter = fragmentedZones.begin() + 1; zoneiter != fragmentedZones.end(); ++zoneiter, ++decreaseiter)
						{
							const EDynamicIndex &zoneIndex = *zoneiter;

							T *currentAddress = &_BufferMemory[zoneIndex.Index];
							T *newAddress = &_BufferMemory[zoneIndex.Index - *decreaseiter];
							mu_memmove(newAddress, currentAddress, sizeof(T) * zoneIndex.Count);

							RequestUpdateDataRaw(zoneIndex.Index - *decreaseiter, zoneIndex.Count);
						}
					}

					_StagingIndex -= decreaseOffsets.back();

					UpdateBuffer();
				}

				_MemoryFragmented = false;
			}
		}
		EINLINE void UpdateBuffer()
		{
			const mu_boolean disabledTransfers = EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false;
			const mu_uint32 currentIndex = EOpenGL::CurrentCommandBufferIndex;

			if (disabledTransfers == false && _BuffersResize[currentIndex] == true)
			{
				glBindBuffer(TargetType, _Buffers[currentIndex]);
				glBufferData(TargetType, _StagingCount * sizeof(T), nullptr, GL_DYNAMIC_DRAW);
				glBufferSubData(TargetType, 0, _StagingIndex * sizeof(T), _BufferMemory);
				glBindBuffer(TargetType, 0);
				_BuffersResize[currentIndex] = false;
			}

			// Update Staging Memory
			if (_StagingUpdateMap.empty() == false)
			{
				const mu_size maxStagingSize = _StagingCount * sizeof(T);

				std::vector<EMappedMemoryRange> mappedRanges;
				ParseMappedRanges(_StagingUpdateMap, mappedRanges, maxStagingSize, sizeof(T));

				for (auto it = _BufferCopies.begin(); it != _BufferCopies.end(); ++it)
				{
					EDynamicBufferCopies &data = *it;
					ParseBufferCopy(mappedRanges, data.Data);
				}

				_StagingUpdateMap.clear();
			}

			EDynamicBufferCopies &copies = _BufferCopies[currentIndex];
			if (disabledTransfers == false && copies.Data.size() > 0)
			{
				glBindBuffer(TargetType, _Buffers[currentIndex]);
				for (auto it = copies.Data.begin(); it != copies.Data.end(); ++it)
				{
					EMappedMemoryRange &tmp = *it;
					glBufferSubData(TargetType, tmp.offset, tmp.size, reinterpret_cast<mu_uint8*>(_BufferMemory) + tmp.offset);
				}
				glBindBuffer(TargetType, 0);
				copies.Data.clear();
			}
		}

		EINLINE const mu_uint32 AllocateDataIndex(const mu_uint32 count)
		{
			if (_MemoryFreeIndexes.empty() == false)
			{
				auto endIter = _MemoryFreeIndexes.end();
				auto freeIter = _MemoryFreeIndexes.end();
				mu_uint32 freeIndex = EInvalidUInt32;
				mu_uint32 freeCount = EInvalidUInt32;

				for (auto it = _MemoryFreeIndexes.begin(); it != _MemoryFreeIndexes.end(); ++it)
				{
					const mu_uint32 index = *it;
					EDynamicIndex &dynamicIndex = _MemoryIndexesMap[index];

					if (dynamicIndex.Count == 0)
					{
						if (freeIter == endIter)
						{
							freeIter = it;
							freeIndex = index;
						}
					}
					else
					{
						if (dynamicIndex.Count >= count &&
							dynamicIndex.Count < freeCount)
						{
							freeIter = it;
							freeIndex = index;
							freeCount = dynamicIndex.Count;
						}
					}
				}

				if (freeIter != endIter)
				{
					EDynamicIndex &dynamicIndex = _MemoryIndexesMap[freeIndex];

					_MemoryFreeIndexes.erase(freeIter);

					if (dynamicIndex.Count > 0)
					{
						if (dynamicIndex.Count > count)
						{
							_MemoryFragmented = true;
						}

						dynamicIndex.Count = count;
					}
					else
					{
						if (_StagingIndex + count > _StagingCount)
						{
							const mu_uint32 diffSize = (_StagingIndex + count) - _StagingCount;
							const mu_uint32 newSize = _StagingCount + diffSize + (GrowSize - diffSize % GrowSize) % GrowSize;
							if (ResizeBuffer(newSize) == false)
							{
								return EInvalidUInt32;
							}
						}

						const mu_uint32 index = _StagingIndex;
						_StagingIndex += count;

						dynamicIndex.Index = index;
						dynamicIndex.Count = count;
					}

					return freeIndex;
				}
			}

			if (_StagingIndex + count > _StagingCount)
			{
				const mu_uint32 diffSize = (_StagingIndex + count) - _StagingCount;
				const mu_uint32 newSize = _StagingCount + diffSize + (GrowSize - diffSize % GrowSize) % GrowSize;
				if (ResizeBuffer(newSize) == false)
				{
					return EInvalidUInt32;
				}
			}

			const mu_uint32 index = _StagingIndex;
			_StagingIndex += count;

			const mu_uint32 freeIndex = _MemoryIndexesMap.size();
			_MemoryIndexesMap.push_back({ index, count });

			return freeIndex;
		}

		EINLINE void DeallocateDataIndex(const mu_uint32 index)
		{
			if (index < _MemoryIndexesMap.size())
			{
				_MemoryFreeIndexes.push_back(index);
				_MemoryFragmented = true;
			}
		}

		EINLINE void RequestUpdateData(const mu_uint32 index, const mu_uint32 count)
		{
			const EDynamicIndex &dynamicIndex = _MemoryIndexesMap[index];

			auto it = _StagingUpdateMap.find(dynamicIndex.Index);
			if (it != _StagingUpdateMap.end())
			{
				if (it->second < count)
				{
					it->second = count;
				}
			}
			else
			{
				_StagingUpdateMap.insert(std::make_pair(dynamicIndex.Index, count));
			}

			UpdateBuffer();
		}

		EINLINE void RequestUpdateDataRaw(const mu_uint32 index, const mu_uint32 count)
		{
			auto it = _StagingUpdateMap.find(index);
			if (it != _StagingUpdateMap.end())
			{
				if (it->second < count)
				{
					it->second = count;
				}
			}
			else
			{
				_StagingUpdateMap.insert(std::make_pair(index, count));
			}
		}

	public:
		EINLINE T *RetrieveDataBuffer(const mu_uint32 index)
		{
			return &_BufferMemory[_MemoryIndexesMap[index].Index];
		}

		EINLINE const mu_uint32 RetrieveBufferBaseIndex(const mu_uint32 index)
		{
			return _MemoryIndexesMap[index].Index;
		}

		EINLINE GLuint RetrieveBuffer(const mu_uint32 index)
		{
			return _Buffers[index];
		}

	private:
		mu_uint32 _StagingIndex;
		mu_uint32 _StagingCount;
		std::map<mu_uint32, mu_uint32> _StagingUpdateMap;

		T *_BufferMemory;
		std::vector<mu_boolean> _BuffersResize;
		std::vector<GLuint> _Buffers;
		std::vector<EDynamicBufferCopies> _BufferCopies;

		struct EDynamicIndex
		{
			mu_uint32 Index;
			mu_uint32 Count;
		};
		mu_boolean _MemoryFragmented;
		std::vector<mu_uint32> _MemoryFreeIndexes;
		std::vector<EDynamicIndex> _MemoryIndexesMap;
	};
};

#endif
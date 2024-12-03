#ifndef __MU_VULKAN_DYNAMICBUFFER_H__
#define __MU_VULKAN_DYNAMICBUFFER_H__

#pragma once

namespace EVulkanUtils
{
	EINLINE EBufferBackend *AllocateBufferBackend()
	{
		return new EBufferBackend();
	}

	struct EMappedMemoryRange
	{
		VkDeviceSize offset;
		VkDeviceSize size;
	};

	EINLINE void ParseMappedRangesFromMap(std::map<mu_uint32, mu_uint32> &Input, std::vector<EMappedMemoryRange> &Output, const VkDeviceSize MaxDeviceSize, const VkDeviceSize DataSize)
	{
		for (auto it = Input.begin(); it != Input.end(); ++it)
		{
			mu_boolean isParsed = false;

			VkDeviceSize leftOffset = it->first * DataSize;
			VkDeviceSize leftSize = it->second * DataSize;
			EVulkan::AlignMappedRangeAtomSize(leftOffset, leftSize, MaxDeviceSize);
			const VkDeviceSize leftEndOffset = leftOffset + leftSize;

			auto mappedit = Output.begin();
			for (; mappedit != Output.end(); ++mappedit)
			{
				EMappedMemoryRange &rightRange = *mappedit;
				const VkDeviceSize rightEndOffset = rightRange.offset + rightRange.size;

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
						EVulkan::AlignMappedRangeAtomSize(rightRange.offset, rightRange.size, MaxDeviceSize);

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

	EINLINE void ParseMappedRangesFromVector(std::vector<EMappedMemoryRange> &Input, std::vector<EMappedMemoryRange> &Output, const VkDeviceSize MaxDeviceSize)
	{
		for (auto it = Input.begin(); it != Input.end(); ++it)
		{
			mu_boolean isParsed = false;

			const EMappedMemoryRange &leftRange = *it;
			const VkDeviceSize leftEndOffset = leftRange.offset + leftRange.size;

			auto mappedit = Output.begin();
			for (; mappedit != Output.end(); ++mappedit)
			{
				EMappedMemoryRange &rightRange = *mappedit;
				const VkDeviceSize rightEndOffset = rightRange.offset + rightRange.size;

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
						EVulkan::AlignMappedRangeAtomSize(rightRange.offset, rightRange.size, MaxDeviceSize);

						isParsed = true;
					}
					break;
				}
				else if (leftRange.offset <= rightEndOffset)
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
				Output.insert(mappedit, leftRange);
			}
		}
	}

	EINLINE void ParseMappedRanges(std::map<mu_uint32, mu_uint32> &Input, std::vector<EMappedMemoryRange> &Output, const VkDeviceSize MaxDeviceSize, const VkDeviceSize DataSize)
	{
		std::vector<EMappedMemoryRange> tmpOutput;
		ParseMappedRangesFromMap(Input, tmpOutput, MaxDeviceSize, DataSize);
		ParseMappedRangesFromVector(tmpOutput, Output, MaxDeviceSize);
	}

	EINLINE void ParseBufferCopy(std::vector<EMappedMemoryRange> &Input, std::vector<EMappedMemoryRange> &Output, const VkDeviceSize MaxDeviceSize)
	{
		std::vector<EMappedMemoryRange> tmp;
		tmp.insert(tmp.end(), Input.begin(), Input.end());
		ParseMappedRangesFromVector(Output, tmp, MaxDeviceSize);
		Output.clear();
		ParseMappedRangesFromVector(tmp, Output, MaxDeviceSize);
	}

	struct EDynamicBufferCopies
	{
		std::vector<EMappedMemoryRange> Data;
	};

	struct EDynamicTrashContainer
	{
		mu_uint32 StagingCount;
		std::map<mu_uint32, mu_uint32> UpdateMap;
		std::vector<EMappedMemoryRange> BufferCopies;
		EBufferBackend *Buffer;
	};

	template<class T, mu_uint32 InitialSize, mu_uint32 GrowSize>
	class EDynamicBuffer
	{
	public:
		EDynamicBuffer()
		{
			_SwapchainCount = 0;
			_StagingIndex = 0;
			_StagingCount = 0;
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
			std::vector<EBufferBackend*> newBuffers(_SwapchainCount, nullptr);

			mu_uint32 n = 0;
			for (; n < _SwapchainCount; ++n)
			{
				newBuffers[n] = new EBufferBackend();

				if (newBuffers[n]->Allocate(count * sizeof(T),
					EBufferUsage::eDynamic,
					EBufferType::eVertex,
					nullptr) == false)
				{
					break;
				}
			}

			if (n < _SwapchainCount)
			{
				for (n = 0; n < _SwapchainCount; ++n)
				{
					if (newBuffers[n] != nullptr)
					{
						newBuffers[n]->Deallocate();
						delete newBuffers[n];
					}
				}
				return false;
			}

			_BufferMemory.resize(count);

			EBufferBackend *oldBuffer = nullptr;
			if (_Buffers.size() > 0 &&
				_Buffers[0] != nullptr)
			{
				_TrashContainer.resize(_TrashContainer.size() + 1);
				EDynamicTrashContainer &trash = _TrashContainer.back();
				trash.StagingCount = _StagingCount;
				trash.UpdateMap.swap(_StagingUpdateMap);
				const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;
				std::copy(_BufferCopies[index].Data.begin(), _BufferCopies[index].Data.end(), trash.BufferCopies.end());
				trash.Buffer = _Buffers[index];
				oldBuffer = trash.Buffer;
			}

			_Buffers.swap(newBuffers);

			for (auto it = newBuffers.begin(); it != newBuffers.end(); ++it)
			{
				EBufferBackend *buffer = *it;
				if (buffer != nullptr &&
					buffer != oldBuffer)
				{
					buffer->Deallocate();
					delete buffer;
				}
			}

			_StagingCount = count;

			return true;
		}

		EINLINE const mu_boolean Initialize(const mu_uint32 SwapchainCount)
		{
			_SwapchainCount = SwapchainCount;
			_BufferCopies.resize(_SwapchainCount);

			return ResizeBuffer(InitialSize);
		}

		EINLINE void Destroy(const mu_boolean gpuOnly = false)
		{
			for (auto it = _Buffers.begin(); it != _Buffers.end(); ++it)
			{
				EBufferBackend *buffer = *it;
				if (buffer != nullptr)
				{
					buffer->Deallocate();
					delete buffer;
					*it = nullptr;
				}
			}

			for (auto it = _TrashContainer.begin(); it != _TrashContainer.end(); ++it)
			{
				EDynamicTrashContainer &trash = *it;

				if (trash.Buffer != nullptr)
				{
					trash.Buffer->Deallocate();
					delete trash.Buffer;
				}
			}
			_TrashContainer.clear();

			if (gpuOnly == false)
			{
				_Buffers.clear();

				_BufferMemory.clear();

				_SwapchainCount = 0;
				_StagingIndex = 0;
				_StagingCount = 0;
				_StagingUpdateMap.clear();
			}
			else
			{
				RequestUpdateData(0, _StagingIndex);
			}
		}

		EINLINE void EmptyCurrentTrash()
		{
			for (auto it = _TrashContainer.begin(); it != _TrashContainer.end(); ++it)
			{
				EDynamicTrashContainer &trash = *it;

				if (trash.Buffer != nullptr)
				{
					trash.Buffer->Deallocate();
					delete trash.Buffer;
				}
			}
			_TrashContainer.clear();
		}

		EINLINE void UpdateBuffer()
		{
			const mu_boolean disabledTransfers = EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false;
			const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;

			// Update Staging Memory
			if (_StagingUpdateMap.empty() == false ||
				_TrashContainer.empty() == false)
			{
				const VkDeviceSize maxStagingSize = _StagingCount * sizeof(T);

				if (_TrashContainer.empty() == false)
				{
					std::vector<EMappedMemoryRange> mappedRanges(1, { 0, maxStagingSize });

					for (mu_uint32 n = 0; n < _SwapchainCount; ++n)
					{
						EDynamicBufferCopies &data = _BufferCopies[n];
						ParseBufferCopy(mappedRanges, data.Data, maxStagingSize);
					}
				}
				else
				{
					std::vector<EMappedMemoryRange> mappedRanges;
					ParseMappedRanges(_StagingUpdateMap, mappedRanges, maxStagingSize, sizeof(T));

					for (mu_uint32 n = 0; n < _SwapchainCount; ++n)
					{
						EDynamicBufferCopies &data = _BufferCopies[n];
						ParseBufferCopy(mappedRanges, data.Data, maxStagingSize);
					}

					_StagingUpdateMap.clear();
				}
			}

			// Copy Trash Memory
			if (_TrashContainer.empty() == false)
			{
				for (auto it = _TrashContainer.begin(); it != _TrashContainer.end(); ++it)
				{
					EDynamicTrashContainer &trash = *it;

					const VkDeviceSize maxStagingSize = trash.StagingCount * sizeof(T);
					std::vector<EMappedMemoryRange> mappedRanges;
					ParseMappedRanges(trash.UpdateMap, mappedRanges, maxStagingSize, sizeof(T));
					ParseBufferCopy(mappedRanges, trash.BufferCopies, maxStagingSize);

					for (auto rangeiter = trash.BufferCopies.begin(); rangeiter != trash.BufferCopies.end(); ++rangeiter)
					{
						EMappedMemoryRange &range = *rangeiter;
						mu_memcpy(reinterpret_cast<mu_uint8*>(trash.Buffer->VulkanInternal.MappedMemory) + range.offset, reinterpret_cast<mu_uint8*>(_BufferMemory.data()) + range.offset, range.size);
					}

					for (auto rangeiter = trash.BufferCopies.begin(); rangeiter != trash.BufferCopies.end(); ++rangeiter)
					{
						EMappedMemoryRange &range = *rangeiter;
						trash.Buffer->Flush(range.offset, range.offset, range.size);
					}

					trash.Buffer->Deallocate();
					delete trash.Buffer;
				}
				_TrashContainer.clear();
			}

			// Copy Memory
			EDynamicBufferCopies &copies = _BufferCopies[index];
			if (disabledTransfers == false && copies.Data.size() > 0)
			{
				EBufferBackend *bufferBackend = _Buffers[index];
				for (auto rangeiter = copies.Data.begin(); rangeiter != copies.Data.end(); ++rangeiter)
				{
					EMappedMemoryRange &range = *rangeiter;
					mu_memcpy(reinterpret_cast<mu_uint8*>(bufferBackend->VulkanInternal.MappedMemory) + range.offset, reinterpret_cast<mu_uint8*>(_BufferMemory.data()) + range.offset, range.size);
				}
				for (auto rangeiter = copies.Data.begin(); rangeiter != copies.Data.end(); ++rangeiter)
				{
					EMappedMemoryRange &range = *rangeiter;
					bufferBackend->Flush(range.offset, range.offset, range.size);
				}
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
		}

	public:
		EINLINE T *RetrieveDataBuffer(const mu_uint32 index)
		{
			return &_BufferMemory[index];
		}

		EINLINE VkBuffer RetrieveBuffer(const mu_uint32 index)
		{
			return _Buffers[index]->VulkanInternal.Buffer;
		}

	private:
		mu_uint32 _SwapchainCount;
		std::vector<T> _BufferMemory;

		mu_uint32 _StagingIndex;
		mu_uint32 _StagingCount;
		std::map<mu_uint32, mu_uint32> _StagingUpdateMap;
		std::vector<EBufferBackend*> _Buffers;
		std::vector<EDynamicBufferCopies> _BufferCopies;

		std::vector<EDynamicTrashContainer> _TrashContainer;
	};

	template<class T, mu_uint32 InitialSize, mu_uint32 GrowSize>
	class EDynamicBufferExtended
	{
	public:
		EDynamicBufferExtended()
		{
			_SwapchainCount = 0;
			_StagingIndex = 0;
			_StagingCount = 0;

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
			std::vector<EBufferBackend*> newBuffers(_SwapchainCount, nullptr);

			mu_uint32 n = 0;
			for (; n < _SwapchainCount; ++n)
			{
				newBuffers[n] = new EBufferBackend();

				if (newBuffers[n]->Allocate(count * sizeof(T),
					EBufferUsage::eDynamic,
					EBufferType::eVertex,
					nullptr) == false)
				{
					break;
				}
			}

			if (n < _SwapchainCount)
			{
				for (n = 0; n < _SwapchainCount; ++n)
				{
					if (newBuffers[n] != nullptr)
					{
						newBuffers[n]->Deallocate();
						delete newBuffers[n];
					}
				}
				return false;
			}

			_BufferMemory.resize(count);

			EBufferBackend *oldBuffer = nullptr;
			if (_Buffers.size() > 0 &&
				_Buffers[0] != nullptr)
			{
				_TrashContainer.resize(_TrashContainer.size() + 1);
				EDynamicTrashContainer &trash = _TrashContainer.back();
				trash.StagingCount = _StagingCount;
				trash.UpdateMap.swap(_StagingUpdateMap);
				const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;
				std::copy(_BufferCopies[index].Data.begin(), _BufferCopies[index].Data.end(), trash.BufferCopies.end());
				trash.Buffer = _Buffers[index];
				oldBuffer = trash.Buffer;
			}

			_Buffers.swap(newBuffers);

			for (auto it = newBuffers.begin(); it != newBuffers.end(); ++it)
			{
				EBufferBackend *buffer = *it;
				if (buffer != nullptr &&
					buffer != oldBuffer)
				{
					buffer->Deallocate();
					delete buffer;
				}
			}

			_StagingCount = count;

			return true;
		}

		EINLINE const mu_boolean Initialize(const mu_uint32 SwapchainCount)
		{
			_SwapchainCount = SwapchainCount;
			_BufferCopies.resize(_SwapchainCount);

			return ResizeBuffer(InitialSize);
		}

		EINLINE void Destroy(const mu_boolean gpuOnly = false)
		{
			for (auto it = _Buffers.begin(); it != _Buffers.end(); ++it)
			{
				EBufferBackend *buffer = *it;
				if (buffer != nullptr)
				{
					buffer->Deallocate();
					delete buffer;
					*it = nullptr;
				}
			}

			for (auto it = _TrashContainer.begin(); it != _TrashContainer.end(); ++it)
			{
				EDynamicTrashContainer &trash = *it;

				if (trash.Buffer != nullptr)
				{
					trash.Buffer->Deallocate();
					delete trash.Buffer;
					trash.Buffer = nullptr;
				}
			}
			_TrashContainer.clear();

			if (gpuOnly == false)
			{
				_Buffers.clear();

				_BufferMemory.clear();

				_SwapchainCount = 0;
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

		EINLINE void EmptyCurrentTrash()
		{
			for (auto it = _TrashContainer.begin(); it != _TrashContainer.end(); ++it)
			{
				EDynamicTrashContainer &trash = *it;

				if (trash.Buffer != nullptr)
				{
					trash.Buffer->Deallocate();
					delete trash.Buffer;
				}
			}
			_TrashContainer.clear();
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
				}

				_MemoryFragmented = false;
			}
		}

		EINLINE void UpdateBuffer()
		{
			const mu_boolean disabledTransfers = EVideoBackend::GetVideoStatus(EVideoStatusFlag::VSF_Available) == false;
			const mu_uint32 index = EVulkan::CurrentCommandBufferIndex;

			// Update Staging Memory
			if (_StagingUpdateMap.empty() == false ||
				_TrashContainer.empty() == false)
			{
				const VkDeviceSize maxStagingSize = _StagingCount * sizeof(T);

				if (_TrashContainer.empty() == false)
				{
					std::vector<EMappedMemoryRange> mappedRanges(1, { 0, maxStagingSize });

					for (mu_uint32 n = 0; n < _SwapchainCount; ++n)
					{
						EDynamicBufferCopies &data = _BufferCopies[n];
						ParseBufferCopy(mappedRanges, data.Data, maxStagingSize);
					}
				}
				else
				{
					std::vector<EMappedMemoryRange> mappedRanges;
					ParseMappedRanges(_StagingUpdateMap, mappedRanges, maxStagingSize, sizeof(T));

					for (mu_uint32 n = 0; n < _SwapchainCount; ++n)
					{
						EDynamicBufferCopies &data = _BufferCopies[n];
						ParseBufferCopy(mappedRanges, data.Data, maxStagingSize);
					}

					_StagingUpdateMap.clear();
				}
			}

			// Copy Trash Memory
			if (_TrashContainer.empty() == false)
			{
				for (auto it = _TrashContainer.begin(); it != _TrashContainer.end(); ++it)
				{
					EDynamicTrashContainer &trash = *it;

					const VkDeviceSize maxStagingSize = trash.StagingCount * sizeof(T);
					std::vector<EMappedMemoryRange> mappedRanges;
					ParseMappedRanges(trash.UpdateMap, mappedRanges, maxStagingSize, sizeof(T));
					ParseBufferCopy(mappedRanges, trash.BufferCopies, maxStagingSize);

					for (auto rangeiter = trash.BufferCopies.begin(); rangeiter != trash.BufferCopies.end(); ++rangeiter)
					{
						EMappedMemoryRange &range = *rangeiter;
						mu_memcpy(reinterpret_cast<mu_uint8*>(trash.Buffer->VulkanInternal.MappedMemory) + range.offset, reinterpret_cast<mu_uint8*>(_BufferMemory.data()) + range.offset, range.size);
					}

					for (auto rangeiter = trash.BufferCopies.begin(); rangeiter != trash.BufferCopies.end(); ++rangeiter)
					{
						EMappedMemoryRange &range = *rangeiter;
						trash.Buffer->Flush(range.offset, range.offset, range.size);
					}

					trash.Buffer->Deallocate();
					delete trash.Buffer;
				}
				_TrashContainer.clear();
			}

			// Copy Memory
			EDynamicBufferCopies &copies = _BufferCopies[index];
			if (disabledTransfers == false && copies.Data.size() > 0)
			{
				EBufferBackend *bufferBackend = _Buffers[index];
				for (auto rangeiter = copies.Data.begin(); rangeiter != copies.Data.end(); ++rangeiter)
				{
					EMappedMemoryRange &range = *rangeiter;
					mu_memcpy(reinterpret_cast<mu_uint8*>(bufferBackend->VulkanInternal.MappedMemory) + range.offset, reinterpret_cast<mu_uint8*>(_BufferMemory.data()) + range.offset, range.size);
				}
				for (auto rangeiter = copies.Data.begin(); rangeiter != copies.Data.end(); ++rangeiter)
				{
					EMappedMemoryRange &range = *rangeiter;
					bufferBackend->Flush(range.offset, range.offset, range.size);
				}
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
						if(freeIter == endIter)
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

		EINLINE VkBuffer RetrieveBuffer(const mu_uint32 index)
		{
			return _Buffers[index]->VulkanInternal.Buffer;
		}

	private:
		mu_uint32 _SwapchainCount;

		std::vector<T> _BufferMemory;

		mu_uint32 _StagingIndex;
		mu_uint32 _StagingCount;
		std::map<mu_uint32, mu_uint32> _StagingUpdateMap;
		std::vector<EBufferBackend*> _Buffers;
		std::vector<EDynamicBufferCopies> _BufferCopies;

		std::vector<EDynamicTrashContainer> _TrashContainer;

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
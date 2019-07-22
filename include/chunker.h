#ifndef chunker_h
#define chunker_h

#include <vector>
#include <iterator>

template<typename T>
class Chunker {
  public:

    Chunker(const T& v, size_t nchunks) : nchunks_(nchunks), 
        size_(v.size()) {
      chunk(v);
    }

    void chunk(const T& v, size_t nchunks) {
      nchunks_ = nchunks;
      chunk(v);
    }

    void chunk(const T& v) {
      size_ = v.size();
      //size_t chunk_size = std::max((size_+nchunks_-1UL)/nchunks_, 1UL);
      size_t pos = 0;
      typename T::const_iterator p = v.begin();
      chunks_.clear();
      chunk_indices_.clear();
      chunks_.push_back(p);
      chunk_indices_.push_back(pos);
      for (size_t i = 0; i < nchunks_; ++i) {
        size_t remain = size_ - pos;
        //size_t chunk_size = std::max((remain)/(nchunks_-i), 1UL);
        size_t chunk_size = (remain)/(nchunks_-i);
        if (chunk_size < 1) chunk_size = 1;
        chunk_size = std::min(chunk_size, remain);
        p = std::next(p, chunk_size);
        pos += chunk_size;
        chunks_.push_back(p);
        chunk_indices_.push_back(pos);
      }
    }

    typename T::const_iterator begin(size_t i) const {
      return chunks_[i];
    }

    typename T::const_iterator end(size_t i) const {
      return chunks_[i+1];
    }

    size_t ibegin(size_t i) const {
      return chunk_indices_[i];
    }

    size_t iend(size_t i) const {
      return chunk_indices_[i+1];
    }

    size_t nchunks() const {
      return nchunks_;
    }

  private:
    size_t nchunks_;
    size_t size_;
    std::vector<typename T::const_iterator> chunks_;
    std::vector<size_t> chunk_indices_;
};

#endif
